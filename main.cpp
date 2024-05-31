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

bool IsCollision(const AABB& aabb1, const AABB& aabb2) {
	if (
		aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x &&
		aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y &&
		aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z
		) {
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
	AABB box1{
		{0,0,0},
		{1,1,1}
	};
	AABB box1World{};
	Vector3 translate1{ Vec3::kZero };

	AABB box2{
		{0,0,0},
		{1,1,1}
	};
	AABB box2World{};
	Vector3 translate2{ Vec3::kZero };

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

		ImGui::Begin("Box1", nullptr, ImGuiWindowFlags_NoSavedSettings);
		ImGui::DragFloat3("Min", &box1.min.x, 0.1f);
		ImGui::DragFloat3("Max", &box1.max.x, 0.1f);
		ImGui::DragFloat3("Translate", &translate1.x, 0.1f);
		{
			auto boxResultX = std::minmax(box1.min.x, box1.max.x);
			auto boxResultY = std::minmax(box1.min.y, box1.max.y);
			auto boxResultZ = std::minmax(box1.min.z, box1.max.z);
			box1.min = { boxResultX.first, boxResultY.first, boxResultZ.first };
			box1.max = { boxResultX.second, boxResultY.second, boxResultZ.second };
		}
		ImGui::End();

		ImGui::Begin("Box2", nullptr, ImGuiWindowFlags_NoSavedSettings);
		ImGui::DragFloat3("Min", &box2.min.x, 0.1f);
		ImGui::DragFloat3("Max", &box2.max.x, 0.1f);
		ImGui::DragFloat3("Translate", &translate2.x, 0.1f);
		{
			auto boxResultX = std::minmax(box2.min.x, box2.max.x);
			auto boxResultY = std::minmax(box2.min.y, box2.max.y);
			auto boxResultZ = std::minmax(box2.min.z, box2.max.z);
			box2.min = { boxResultX.first, boxResultY.first, boxResultZ.first };
			box2.max = { boxResultX.second, boxResultY.second, boxResultZ.second };
		}
		ImGui::End();

		box1World.min = box1.min + translate1;
		box1World.max = box1.max + translate1;
		box2World.min = box2.min + translate2;
		box2World.max = box2.max + translate2;

		if (IsCollision(box1World, box2World)) {
			color = { 1.0f,0.0f,0.0f,1.0f };
		}
		else {
			color = { 1.0f,1.0f,1.0f,1.0f };
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
		DrawAABB(box1World, hexColor);
		DrawAABB(box2World, hexColor);

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
