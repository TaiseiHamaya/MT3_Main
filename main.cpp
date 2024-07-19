#include <Novice.h>

#include <Camera3D.h>
#include <DeviceData.h>

#include <algorithm>
#include <cmath>

#include <Vector3D.h>
#include <Matrix.h>
#include <Debug.h>
#include <Transform3D.h>
#include <Sphere.h>
#include <Renderer.h>
#include "imgui.h"

const char kWindowTitle[] = "LE2A_14_ハマヤ_タイセイ_MT3";
float deltaTime;

struct Plane {
	Vector3 normal; // 法線
	float distance; // 距離
};

struct Ball {
	Sphere sphere;
	Vector3 velocity;
	Vector3 acceleration;
	float mass;
};

bool IsCollision(const Plane& plane, const Ball& ball) {
	float dot = Vector3::DotProduct(plane.normal, -ball.velocity * deltaTime);

	if (dot == 0) {
		return false;
	}
	float t = (plane.distance - Vector3::DotProduct(plane.normal, ball.sphere.get_transform().get_translate())) / dot;

	Vector3 nearest = ball.sphere.get_transform().get_translate() - ball.velocity * deltaTime * std::clamp(t, 0.0f, 1.0f);

	float distance = std::abs(Vector3::DotProduct(plane.normal, nearest) - plane.distance);
	return distance <= ball.sphere.get_radius();
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// -------------------------------------------ライブラリの初期化------------------------------------------
	Novice::Initialize(kWindowTitle, 1280, 720);

	DeviceData::Initialize(1280, 720);
	Camera3D::Initialize();
	Camera3D::SetCameraTransform({ Vec3::kBasis, Quaternion{0.45f, 0.0f,0.0f }, {0,12.0f,-26.0f} });
	Camera3D::CameraUpdate();

	// --------------------------------------------変数宣言--------------------------------------------
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	deltaTime = 1.0f / 60;
	const Vector3 gravity{ 0,-9.8f,0.0f };

	Plane plane;
	plane.normal = -Vector3::Normalize({ -0.3f, 1.2f, -0.4f });
	//plane.normal = Vec3::kBasisY;
	plane.distance = 1;

	float e = 0.8f;

	Vector3 resetPosition = { 0.8f,1.2f,0.3f };

	Ball ball;
	ball.sphere = { {}, { 0.0f, 0.0f, 1.0f, 1.0f }, 0.05f, 16 };
	ball.sphere.get_transform().set_translate(resetPosition);
	ball.mass = 2.0f;

	ball.acceleration = gravity;

	// ---------------------------------------------ゲームループ---------------------------------------------
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力の記録
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ----------------------------------------更新処理ここから----------------------------------------
		///

		ImGui::Begin("Debug");
		if (ImGui::Button("Reset")) {
			ball.sphere.get_transform().set_translate(resetPosition);
			ball.velocity = Vec3::kZero;
		}
		ImGui::DragFloat3("ResetPosition", &resetPosition.x, 0.05f);
		ImGui::End();

		ball.velocity += ball.acceleration * deltaTime;
		Vector3 fromPosition = ball.sphere.get_transform().get_translate();
		ball.sphere.get_transform().plus_translate(ball.velocity * deltaTime);

		if (IsCollision(plane, ball)) {
			// 球が存在した方向を取得
			float sign = Vector3::DotProduct(plane.normal, fromPosition) - plane.distance < 0 ? -1.0f : 1.0f;
			
			float dot = Vector3::DotProduct(plane.normal, -ball.velocity * deltaTime);
			// sign * radius分面の位置を動かし、衝突時の中心点を求める
			float t = (plane.distance + (sign * ball.sphere.get_radius()) - Vector3::DotProduct(plane.normal, ball.sphere.get_transform().get_translate())) / dot;
			// 戻す量
			Vector3 penetration = -ball.velocity * deltaTime * t;
			// 平行成分と垂直成分に分離
			Vector3 vertical = Vector3::Projection(penetration, plane.normal);
			Vector3 parallel = vertical - penetration;
			// 接触時まで戻してさらに反発分動く
			Vector3 nextPosition = ball.sphere.get_transform().get_translate() + penetration + vertical * e + parallel;
			
			// Velocity計算
			// 反射ベクトルを求める
			Vector3 refrected = Vector3::Reflect(ball.velocity , plane.normal);
			// 分離
			Vector3 projNorm = Vector3::Projection(refrected, plane.normal);
			Vector3 moveDirection = refrected - projNorm;
			// 垂直には反発係数eをかける。反発分の動き中のvelocityを適用。
			ball.velocity = projNorm * e + moveDirection + ball.acceleration * t * deltaTime;

			// 最後に位置も修正
			ball.sphere.get_transform().set_translate(nextPosition);
		}

		Camera3D::DebugGUI();
		Camera3D::CameraUpdate();
		ball.sphere.begin_rendering();

		///
		/// ----------------------------------------更新処理ここまで----------------------------------------
		///

		///
		/// ----------------------------------------描画処理ここから----------------------------------------
		///

		Debug::Grid3D();

		ball.sphere.draw();

		{
			Vector3 center = plane.normal * plane.distance;
			std::array<Vector3, 4> planeVertexes;
			planeVertexes[0] =
				plane.normal.x != 0 || plane.normal.y != 0
				? Vector3{ -plane.normal.y, plane.normal.x, 0.0f } : Vector3{ 0.0f, -plane.normal.z, plane.normal.y };
			planeVertexes[0] = planeVertexes[0].normalize() * 10;
			planeVertexes[2] = -planeVertexes[0];
			planeVertexes[1] = Vector3::CrossProduct(planeVertexes[0], plane.normal);
			planeVertexes[3] = -planeVertexes[1];


			std::array<Vector3, 4> planeScreenVertexes;

			for (int i = 0; i < planeVertexes.size(); ++i) {
				planeScreenVertexes[i] = Transform3D::Homogeneous(planeVertexes[i] + center, Camera3D::GetVPOVMatrix());
			}

			Renderer::DrawLine(planeScreenVertexes[0], planeScreenVertexes[1], WHITE);
			Renderer::DrawLine(planeScreenVertexes[1], planeScreenVertexes[2], WHITE);
			Renderer::DrawLine(planeScreenVertexes[2], planeScreenVertexes[3], WHITE);
			Renderer::DrawLine(planeScreenVertexes[3], planeScreenVertexes[0], WHITE);

		}

		///
		/// ----------------------------------------描画処理ここまで----------------------------------------
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
