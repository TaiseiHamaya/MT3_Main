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

	Vector3 anglarVelocity = { 0,0,PI };
	float deltaTime = 1.0f / 60;
	const Vector3 gravity{ 0,-9.8f,0.0f };

	Ball ball;
	ball = {
		{1.2f, 0.0f, 0.0f},
		Vec3::kZero,
		Vec3::kZero,
		2.0f
	};

	ball.velocity = Vector3::CrossProduct(anglarVelocity, ball.position);

	Sphere sphere{ {}, {0.0f, 0.0f, 1.0f, 1.0f},0.05f,16 };

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

		float angle2 = Vector3::DotProduct(anglarVelocity, anglarVelocity);
		Vector3 centripetal = ball.position * -angle2;
		ball.acceleration = centripetal;
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
