#include <Novice.h>

#include <Camera3D.h>
#include <DeviceData.h>

#include <algorithm>

#include <Vector3D.h>
#include <Matrix.h>
#include <Debug.h>
#include <Transform3D.h>
#include <Sphere.h>
#include <Renderer.h>
#include "imgui.h"

const char kWindowTitle[] = "LE2A_14_ハマヤ_タイセイ_MT3";

struct Spring {
	Vector3 anchor;
	float naturalLength;
	float stiffness;
	float danpingConfficient;
};

struct Ball {
	Vector3 position;
	Vector3 velocity;
	Vector3 acceleration;
	float mass;
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// -------------------------------------------ライブラリの初期化------------------------------------------
	Novice::Initialize(kWindowTitle, 1280, 720);

	DeviceData::Initialize(1280, 720);
	Camera3D::Initialize();
	Camera3D::SetCameraTransform({ Vec3::kBasis, Quaternion{0.45f, 0.0f,0.0f }, {0,6.0f,-12.0f} });
	Camera3D::CameraUpdate();

	// --------------------------------------------変数宣言--------------------------------------------
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Spring spring;
	Ball ball;

	spring = {
		{0,1,0},
		1.0f,
		100.0f,
		2.0f
	};
	ball = {
		{1.2f, 0.0f, 0.0f},
		Vec3::kZero, Vec3::kZero,
		2.0f
	};

	Sphere sphere{ {}, {0.0f, 0.0f, 1.0f, 1.0f},0.05f,16 };
	float deltaTime = 1.0f / 60;
	const Vector3 gravity{ 0,-9.8f,0.0f };

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
		if (ImGui::Button("Start")) {
			ball = {
				{1.2f, 0.0f, 0.0f},
				Vec3::kZero, Vec3::kZero,
				2.0f
			};
		}
		ImGui::End();

		Vector3 newAcc;

		Vector3 diff = ball.position - spring.anchor;
		float diffLength = diff.length();
		if (diffLength != 0) {
			Vector3 restPosition = spring.anchor + diff.normalize() * spring.naturalLength;
			newAcc = (ball.position - restPosition) * (-spring.stiffness * diffLength) / ball.mass + ball.velocity * -spring.danpingConfficient;
		}

		newAcc += gravity * ball.mass;

		ball.acceleration = newAcc;
		ball.velocity += ball.acceleration * deltaTime;
		ball.position += ball.velocity * deltaTime;
		sphere.get_transform().set_translate(ball.position);

		Camera3D::DebugGUI();
		Camera3D::CameraUpdate();
		sphere.begin_rendering();

		///
		/// ----------------------------------------更新処理ここまで----------------------------------------
		///

		///
		/// ----------------------------------------描画処理ここから----------------------------------------
		///

		Debug::Grid3D();

		Renderer::DrawLine(
			Transform3D::Homogeneous(spring.anchor, Camera3D::GetVPOVMatrix()),
			Transform3D::Homogeneous(ball.position, Camera3D::GetVPOVMatrix()),
			0xffffffff
		);

		sphere.draw();

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
