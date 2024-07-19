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

struct Plane {
	Vector3 normal; // 法線
	float distance; // 距離
};

struct Segment {
public:
	Vector3 origin; // 原点
	Vector3 diff; // 向き
};

struct Ball {
	Sphere sphere;
	Vector3 velocity;
	Vector3 acceleration;
	float mass;
};

bool IsCollision(const Plane& plane, const Segment& segment) {
	float dot = Vector3::DotProduct(plane.normal, segment.diff);

	if (dot == 0) {
		return false;
	}
	float t = (plane.distance - Vector3::DotProduct(plane.normal, segment.origin)) / dot;

	return t >= 0 && t <= 1;
}

bool IsCollision(const Plane& plane, const Sphere& sphere) {
	float distance = std::abs(Vector3::DotProduct(plane.normal, sphere.get_transform().get_translate()) - plane.distance);
	return distance <= sphere.get_radius();
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

	float deltaTime = 1.0f / 60;
	const Vector3 gravity{ 0,-9.8f,0.0f };

	Plane plane;
	plane.normal = Vector3::Normalize({ -0.2f, 0.9f, -0.3f });
	plane.distance = 0;

	float e = 0.8f;

	Ball ball;
	ball.sphere = { {}, { 0.0f, 0.0f, 1.0f, 1.0f }, 0.05f, 16 };
	ball.sphere.get_transform().set_translate({ 0.8f,1.2f,0.3f });
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

		ball.velocity += ball.acceleration * deltaTime;
		ball.sphere.get_transform().plus_translate(ball.velocity * deltaTime);

		if (IsCollision(plane, ball.sphere)) {
			Vector3 refrected = Vector3::Reflect(ball.velocity, plane.normal);
			Vector3 projNorm = Vector3::Projection(refrected, plane.normal);
			Vector3 moveDirection = refrected - projNorm;
			ball.velocity = projNorm * e + moveDirection;
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
