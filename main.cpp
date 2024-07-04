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

	Transform3D shoulder{ Vec3::kBasis, Quaternion{0,0,-6.8f},{0.2f, 1.0f, 0.0f} };
	Vector3 shoulderWorldPosition;
	Sphere shoulderScreen{ {},{1.0f,0.0f,0.0f,1.0f},0.1f, 16 };
	Transform3D elbow{ Vec3::kBasis, Quaternion{0,0,-1.4f},{0.4f, 0.0f, 0.0f} };
	Vector3 elbowWorldPosition;
	Sphere elbowScreen{ {},{0.0f,1.0f,0.0f,1.0f},0.1f, 16 };
	Transform3D hand{ Vec3::kBasis, Quaternion{0,0,0},{0.3f, 0.0f, 0.0f} };
	Vector3 handWorldPosition;
	Sphere handScreen{ {},{0.0f,0.0f,1.0f,1.0f},0.1f, 16 };

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

		ImGui::Begin("Window");
		if (ImGui::TreeNodeEx("Shoulder", ImGuiTreeNodeFlags_DefaultOpen)) {
			shoulder.debug_gui();
			ImGui::TreePop();
		}
		ImGui::Separator();
		if (ImGui::TreeNodeEx("Elbow", ImGuiTreeNodeFlags_DefaultOpen)) {
			elbow.debug_gui();
			ImGui::TreePop();
		}
		ImGui::Separator();
		if (ImGui::TreeNodeEx("Hand", ImGuiTreeNodeFlags_DefaultOpen)) {
			hand.debug_gui();
			ImGui::TreePop();
		}
		ImGui::End();

		shoulder.update();
		elbow.update();
		hand.update();

		shoulderWorldPosition = Transform3D::Homogeneous(Vec3::kZero, shoulder.get_matrix());
		elbowWorldPosition = Transform3D::Homogeneous(Vec3::kZero, elbow.get_matrix() * shoulder.get_matrix());
		handWorldPosition = Transform3D::Homogeneous(Vec3::kZero, hand.get_matrix()* elbow.get_matrix()* shoulder.get_matrix());

		shoulderScreen.get_transform().set_translate(shoulderWorldPosition);
		elbowScreen.get_transform().set_translate(elbowWorldPosition);
		handScreen.get_transform().set_translate(handWorldPosition);

		shoulderScreen.begin_rendering();
		elbowScreen.begin_rendering();
		handScreen.begin_rendering();

		///
		/// ----------------------------------------更新処理ここまで----------------------------------------
		///

		///
		/// ----------------------------------------描画処理ここから----------------------------------------
		///

		Debug::Grid3D();

		shoulderScreen.draw();
		elbowScreen.draw();
		handScreen.draw();

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
