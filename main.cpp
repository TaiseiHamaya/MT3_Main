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

struct AABB {
	Vector3 min;
	Vector3 max;
};

void DrawAABB(const AABB& aabb, unsigned int color) {
	std::array<Vector3, 8> screenPos;
	screenPos[0] = Transform3D::Homogeneous(aabb.min, Camera3D::GetVPOVMatrix());
	screenPos[1] = Transform3D::Homogeneous({ aabb.min.x,aabb.max.y,aabb.min.z }, Camera3D::GetVPOVMatrix());
	screenPos[2] = Transform3D::Homogeneous({ aabb.max.x,aabb.min.y,aabb.min.z }, Camera3D::GetVPOVMatrix());
	screenPos[3] = Transform3D::Homogeneous({ aabb.max.x,aabb.max.y,aabb.min.z }, Camera3D::GetVPOVMatrix());
	screenPos[4] = Transform3D::Homogeneous({ aabb.min.x,aabb.min.y,aabb.max.z }, Camera3D::GetVPOVMatrix());
	screenPos[5] = Transform3D::Homogeneous({ aabb.min.x,aabb.max.y,aabb.max.z }, Camera3D::GetVPOVMatrix());
	screenPos[6] = Transform3D::Homogeneous({ aabb.max.x,aabb.min.y,aabb.max.z }, Camera3D::GetVPOVMatrix());
	screenPos[7] = Transform3D::Homogeneous({ aabb.max.x,aabb.max.y,aabb.max.z }, Camera3D::GetVPOVMatrix());

	Renderer::DrawLine(screenPos[0], screenPos[1], color);
	Renderer::DrawLine(screenPos[0], screenPos[2], color);
	Renderer::DrawLine(screenPos[1], screenPos[3], color);
	Renderer::DrawLine(screenPos[2], screenPos[3], color);

	Renderer::DrawLine(screenPos[4], screenPos[5], color);
	Renderer::DrawLine(screenPos[4], screenPos[6], color);
	Renderer::DrawLine(screenPos[5], screenPos[7], color);
	Renderer::DrawLine(screenPos[6], screenPos[7], color);

	Renderer::DrawLine(screenPos[0], screenPos[4], color);
	Renderer::DrawLine(screenPos[1], screenPos[5], color);
	Renderer::DrawLine(screenPos[2], screenPos[6], color);
	Renderer::DrawLine(screenPos[3], screenPos[7], color);
}

bool IsCollision(const AABB& aabb, const Sphere& sphere) {
	Vector3 closest = Vector3::Clamp(sphere.get_transform().get_translate(), aabb.min, aabb.max);
	if ((closest - sphere.get_transform().get_translate()).length() <= sphere.get_radius()) {
		return true;
	}
	return false;
}

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

	Color color = { 1.0f,1.0f,1.0f,1.0f };
	AABB box{
		{0,0,0},
		{1,1,1}
	};
	AABB boxWorld{};
	Vector3 translate1{ Vec3::kZero };

	Sphere sphere{ {},{},2,16 };

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

		ImGui::SetNextWindowSize(ImVec2{ 300,100 }, ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2{ 50, 220 }, ImGuiCond_Once);
		ImGui::Begin("Box", nullptr, ImGuiWindowFlags_NoSavedSettings);
		ImGui::DragFloat3("Min", &box.min.x, 0.1f);
		ImGui::DragFloat3("Max", &box.max.x, 0.1f);
		ImGui::DragFloat3("Translate", &translate1.x, 0.1f);
		{
			auto boxResultX = std::minmax(box.min.x, box.max.x);
			auto boxResultY = std::minmax(box.min.y, box.max.y);
			auto boxResultZ = std::minmax(box.min.z, box.max.z);
			box.min = { boxResultX.first, boxResultY.first, boxResultZ.first };
			box.max = { boxResultX.second, boxResultY.second, boxResultZ.second };
		}
		ImGui::End();

		ImGui::SetNextWindowSize(ImVec2{ 300,210 }, ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2{ 50, 340 }, ImGuiCond_Once);
		ImGui::Begin("Sphere", nullptr, ImGuiWindowFlags_NoSavedSettings);
		sphere.debug_gui();
		ImGui::End();
		sphere.update();
		sphere.begin_rendering();

		boxWorld.min = box.min + translate1;
		boxWorld.max = box.max + translate1;

		if (IsCollision(boxWorld, sphere)) {
			color = { 1.0f,0.0f,0.0f,1.0f };
			sphere.set_color(color);
		}
		else {
			color = { 1.0f,1.0f,1.0f,1.0f };
			sphere.set_color(color);
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

		unsigned int hexColor = color.hex();
		Debug::Grid3D();
		DrawAABB(boxWorld, hexColor);
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
