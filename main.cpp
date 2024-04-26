#include <Novice.h>

#include <Camera3D.h>
#include <DeviceData.h>

#include <Quaternion.h>
#include <Vector3D.h>
#include <Matrix.h>
#include <Debug.h>
#include <Transform3D.h>
#include "imgui.h"

const char kWindowTitle[] = "LE2A_14_ハマヤ_タイセイ_MT3";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// -------------------------------------------ライブラリの初期化------------------------------------------
	Novice::Initialize(kWindowTitle, 1280, 720);

	DeviceData::Initialize(1280, 720);
	Camera3D::Initialize();
	Camera3D::CameraUpdate();

	// --------------------------------------------変数宣言--------------------------------------------
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	std::array<Vector3, 3> triangle;
	std::array<Vector3, 3> worldTriangle;
	std::array<Vector3, 3> screenTriangle;
	std::unique_ptr<Transform3D> transform;
	triangle = {
		Vector3{0,1,0},
		Vector3{1,-1,0},
		Vector3{-1,-1,0}
	};
	transform = std::make_unique<Transform3D>(Vec3::kBasis, Quaternion{ 0,0,0 }, Vector3{ 0,0,0 });

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

		ImGui::SetNextWindowSize(ImVec2{ 300,125 }, ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2{ 900, 100 }, ImGuiCond_Once);
		ImGui::Begin("Triangle1", nullptr, ImGuiWindowFlags_NoSavedSettings);
		transform->debug_gui();
		ImGui::End();

		Camera3D::DebugGUI();

		transform->update();
		for (int i = 0; i < 3; ++i) {
			worldTriangle[i] = Transform3D::Homogeneous(triangle[i], transform->get_matrix());
			screenTriangle[i] = Transform3D::Homogeneous(worldTriangle[i], Camera3D::GetVPOVMatrix());
		}
		Camera3D::CameraUpdate();

		transform->update();
		for (int i = 0; i < 3; ++i) {
			worldTriangle[i] = Transform3D::Homogeneous(triangle[i], transform->get_matrix());
			screenTriangle[i] = Transform3D::Homogeneous(worldTriangle[i], Camera3D::GetVPOVMatrix());
		}
		Camera3D::CameraUpdate();

		///
		/// ----------------------------------------更新処理ここまで----------------------------------------
		///

		///
		/// ----------------------------------------描画処理ここから----------------------------------------
		///

		if (Vector3::DotProduct(transform->get_translate() - Camera3D::GetCameraTransform().get_translate(), Vector3::CrossProduct(worldTriangle[1] - worldTriangle[0], worldTriangle[2] - worldTriangle[1])) <= 0) {
			Novice::DrawTriangle(
				int(screenTriangle[0].x), int(screenTriangle[0].y),
				int(screenTriangle[1].x), int(screenTriangle[1].y),
				int(screenTriangle[2].x), int(screenTriangle[2].y),
				WHITE, kFillModeSolid
			);
		}
		else {
			Novice::DrawTriangle(
				int(screenTriangle[0].x), int(screenTriangle[0].y),
				int(screenTriangle[1].x), int(screenTriangle[1].y),
				int(screenTriangle[2].x), int(screenTriangle[2].y),
				WHITE, kFillModeWireFrame
			);
		}
		transform->debug_axis(transform->get_matrix() * Camera3D::GetVPOVMatrix());

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
