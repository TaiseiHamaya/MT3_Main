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

Vector3 CatmullRom(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t) {
	return (
		(-p0 + p1 * 3 - p2 * 3 + p3) * t * t * t +
		(p0 * 2 - p1 * 5 + p2 * 4 - p3) * t * t +
		(-p0 + p2) * t +
		p1 * 2) / 2;
}

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

	std::array<Vector3, 4> controlPoint{
		Vector3{-0.8f, 0.58f, 1.0f},
		Vector3{1.76f, 1.0f, -0.3f},
		Vector3{0.94f, -0.7f, 2.3f},
		Vector3{-0.53f, 0.26f, -0.15f}
	};

	std::array<Sphere, 4> controlPointSphere{
		Sphere{Transform3D{}, color, 0.1f, 8},
		Sphere{Transform3D{}, color, 0.1f, 8},
		Sphere{Transform3D{}, color, 0.1f, 8},
		Sphere{Transform3D{}, color, 0.1f, 8},
	};
	controlPointSphere[0].get_transform().set_translate(controlPoint[0]);
	controlPointSphere[1].get_transform().set_translate(controlPoint[1]);
	controlPointSphere[2].get_transform().set_translate(controlPoint[2]);
	controlPointSphere[3].get_transform().set_translate(controlPoint[3]);

	std::vector<Vector3> spline;
	std::vector<Vector3> splineScreen;

	constexpr int kDivision = 30;
	spline.resize(kDivision * (controlPoint.size() - 1) + 1);
	splineScreen.resize(kDivision * (controlPoint.size() - 1) + 1);

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

		// 中間2
		if (ImGui::DragFloat3("Point2", &controlPoint[2].x, 0.05f)) {
			controlPointSphere[2].get_transform().set_translate(controlPoint[2]);
		}

		// 終点
		if (ImGui::DragFloat3("Point3", &controlPoint[3].x, 0.05f)) {
			controlPointSphere[3].get_transform().set_translate(controlPoint[3]);
		}
		ImGui::End();

		// ベジエの更新
		for (int i = 0; i < kDivision; ++i) {
			spline[i] = CatmullRom(controlPoint[0], controlPoint[0], controlPoint[1], controlPoint[2], float(i) / kDivision);
		}
		for (int i = 0; i < kDivision; ++i) {
			spline[i + kDivision] = CatmullRom(controlPoint[0], controlPoint[1], controlPoint[2], controlPoint[3], float(i) / kDivision);
		}
		for (int i = 0; i < kDivision; ++i) {
			spline[i + kDivision * 2] = CatmullRom(controlPoint[1], controlPoint[2], controlPoint[3], controlPoint[3], float(i) / kDivision);
		}
		spline[kDivision * 3] = CatmullRom(controlPoint[1], controlPoint[2], controlPoint[3], controlPoint[3], 1);
		for (int i = 0; i < kDivision * 3 + 1; ++i) {
			splineScreen[i] = Transform3D::Homogeneous(spline[i], Camera3D::GetVPOVMatrix());
		}

		controlPointSphere[0].begin_rendering();
		controlPointSphere[1].begin_rendering();
		controlPointSphere[2].begin_rendering();
		controlPointSphere[3].begin_rendering();

		///
		/// ----------------------------------------更新処理ここまで----------------------------------------
		///

		///
		/// ----------------------------------------描画処理ここから----------------------------------------
		///

		Debug::Grid3D();
		for (int i = 0; i < kDivision * 3; ++i) {
			Renderer::DrawLine(splineScreen[i], splineScreen[i + 1], color.hex());
		}
		controlPointSphere[0].draw();
		controlPointSphere[1].draw();
		controlPointSphere[2].draw();
		controlPointSphere[3].draw();

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
