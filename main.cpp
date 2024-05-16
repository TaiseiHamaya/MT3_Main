#include <Novice.h>

#include <Camera3D.h>
#include <DeviceData.h>

#include <Quaternion.h>
#include <Vector3D.h>
#include <Matrix.h>
#include <Debug.h>
#include <Transform3D.h>
#include <Sphere.h>
#include <Renderer.h>
#include "imgui.h"

#include <algorithm>

const char kWindowTitle[] = "LE2A_14_ハマヤ_タイセイ_MT3";

bool IsCollision(const Sphere& s1, const Sphere& s2) {
	return (s1.get_transform().get_translate() - s2.get_transform().get_translate()).length() <= s1.get_radius() + s2.get_radius();
};

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

	Sphere sphere1{ Transform3D{ Vec3::kBasis, Vec3::kZero, {-1.5f, 0.6f, 0.6f }}, Color{ 1.0f, 1.0f, 1.0f, 1.0f }, 1.f, 16 };
	Sphere sphere2{ Transform3D{ Vec3::kBasis, Vec3::kZero, Vec3::kZero }, Color{ 1.0f, 1.0f, 1.0f, 1.0f }, 1.f, 16 };

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

		// s1
		sphere1.begin();
		ImGui::SetNextWindowSize(ImVec2{ 300,210 }, ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2{ 900, 100 }, ImGuiCond_Once);
		ImGui::Begin("Sphere1", nullptr, ImGuiWindowFlags_NoSavedSettings);
		sphere1.debug_gui();
		ImGui::End();
		sphere1.update();
		sphere1.begin_rendering();

		// s2
		sphere2.begin();
		ImGui::SetNextWindowSize(ImVec2{ 300,210 }, ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2{ 900, 350 }, ImGuiCond_Once);
		ImGui::Begin("Sphere2", nullptr, ImGuiWindowFlags_NoSavedSettings);
		sphere2.debug_gui();
		ImGui::End();
		sphere2.update();
		sphere2.begin_rendering();

		if (IsCollision(sphere1, sphere2)) {
			sphere1.set_color({ 1.0f,0.0f,0.0f,1.0f });
			sphere2.set_color({ 1.0f,0.0f,0.0f,1.0f });
		}
		else {
			sphere1.set_color({ 1.0f,1.0f,1.0f,1.0f });
			sphere2.set_color({ 1.0f,1.0f,1.0f,1.0f });
		}

		// カメラ関連
		Camera3D::DebugGUI();
		Camera3D::CameraUpdate();

		///
		/// ----------------------------------------更新処理ここまで----------------------------------------
		///

		///
		/// ----------------------------------------描画処理ここから----------------------------------------
		///

		Debug::Grid3D();
		sphere1.draw();
		sphere2.draw();


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
