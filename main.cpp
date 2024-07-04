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

	Color color = { 1.0f,1.0f,1.0f,1.0f };

	std::array<Vector3, 3> controlPoint{
		Vector3{-0.8f, 0.58f, 1.0f},
		Vector3{1.76f, 1.0f, -0.3f},
		Vector3{0.94f, -0.7f, 2.3f},
	};

	std::array<Sphere, 3> controlPointSphere{
		Sphere{Transform3D{}, color, 0.1f, 8},
		Sphere{Transform3D{}, color, 0.1f, 8},
		Sphere{Transform3D{}, color, 0.1f, 8},
	};
	controlPointSphere[0].get_transform().set_translate(controlPoint[0]);
	controlPointSphere[1].get_transform().set_translate(controlPoint[1]);
	controlPointSphere[2].get_transform().set_translate(controlPoint[2]);

	std::vector<Vector3> bezier;
	std::vector<Vector3> bezierScreen;

	constexpr int kDivision = 100;
	bezier.resize(kDivision);
	bezierScreen.resize(kDivision);

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

		// カメラ関連
		Camera3D::DebugGUI();
		Camera3D::CameraUpdate();

		// ControlPointのImGui
		ImGui::Begin("ControlPoints");
		// 始点
		if (ImGui::DragFloat3("Point0", &controlPoint[0].x, 0.05f)) {
			controlPointSphere[0].get_transform().set_translate(controlPoint[0]);
		}

		// 中間
		if (ImGui::DragFloat3("Point1", &controlPoint[1].x, 0.05f)) {
			controlPointSphere[1].get_transform().set_translate(controlPoint[1]);
		}

		// 終点
		if (ImGui::DragFloat3("Point2", &controlPoint[2].x, 0.05f)) {
			controlPointSphere[2].get_transform().set_translate(controlPoint[2]);
		}
		ImGui::End();

		// ベジエの更新
		for (int i = 0; i < kDivision; ++i) {
			bezier[i] = (Vector3::Bezier(controlPoint[0], controlPoint[1], controlPoint[2], float(i) / kDivision));
		}
		for (int i = 0; i < kDivision; ++i) {
			bezierScreen[i] = Transform3D::Homogeneous(bezier[i], Camera3D::GetVPOVMatrix());
		}

		controlPointSphere[0].begin_rendering();
		controlPointSphere[1].begin_rendering();
		controlPointSphere[2].begin_rendering();

		///
		/// ----------------------------------------更新処理ここまで----------------------------------------
		///

		///
		/// ----------------------------------------描画処理ここから----------------------------------------
		///

		Debug::Grid3D();
		for (int i = 0; i < kDivision - 1; ++i) {
			Renderer::DrawLine(bezierScreen[i], bezierScreen[i + 1], color.hex());
		}
		controlPointSphere[0].draw();
		controlPointSphere[1].draw();
		controlPointSphere[2].draw();

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
