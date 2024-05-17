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

struct Plane {
	Vector3 normal; // 法線
	float distance; // 距離
};

struct Segment {
public:
	Vector3 origin; // 原点
	Vector3 diff; // 向き
};

bool IsCollision(const Plane& plane, const Segment& segment) {
	float dot = Vector3::DotProduct(plane.normal, segment.diff);

	if (dot == 0) {
		return false;
	}
	float t = (plane.distance - Vector3::DotProduct(plane.normal, segment.origin)) / dot;

	return t >= 0 && t <= 1;
}

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

	Segment segment{ { -2.0f, -1.0f, 0.0f },{ 3.0f, 2.0f, 2.0f } };
	Plane plane{ Vec3::kBasisY, 1.0f };
	Color segmentColor = { 1.0f,1.0f,1.0f,1.0f };

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

		// 線分
		ImGui::SetNextWindowSize(ImVec2{ 300,185 }, ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2{ 900, 100 }, ImGuiCond_Once);
		ImGui::Begin("Segment", nullptr, ImGuiWindowFlags_NoSavedSettings);
		ImGui::DragFloat3("Origin", &segment.origin.x, 0.1f);
		ImGui::DragFloat3("Diff", &segment.diff.x, 0.1f);
		ImGui::End();


		// plane
		ImGui::SetNextWindowSize(ImVec2{ 300,210 }, ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2{ 900, 350 }, ImGuiCond_Once);
		ImGui::Begin("Sphere2", nullptr, ImGuiWindowFlags_NoSavedSettings);
		if (ImGui::DragFloat3("DirectionRotateV1", &plane.normal.x, 0.01f)) {
			plane.normal = plane.normal.normalize();
		}
		Vector3 rotate = Vec3::kZero;
		if (ImGui::DragFloat3("DirectionRotateV2", &rotate.x, 0.02f)) {
			plane.normal = Transform3D::Homogeneous(plane.normal, Quaternion{ rotate }.to_matrix());
			plane.normal = plane.normal.normalize();
		}
		if (ImGui::Button("ResetNormal")) {
			plane.normal = Vec3::kBasisY;
		}
		ImGui::Text("%.3f, %.3f, %.3f", plane.normal.x, plane.normal.y, plane.normal.z);
		ImGui::DragFloat("Distance", &plane.distance, 0.1f);
		ImGui::End();

		if (IsCollision(plane, segment)) {
			segmentColor = { 1.0f,0.0f,0.0f,1.0f };
		}
		else {
			segmentColor = { 1.0f,1.0f,1.0f,1.0f };
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

		unsigned int color = segmentColor.hex();
		Debug::Grid3D();
		Vector3 start = Transform3D::Homogeneous(segment.origin, Camera3D::GetVPOVMatrix());
		Vector3 end = Transform3D::Homogeneous(segment.origin + segment.diff, Camera3D::GetVPOVMatrix());
		Renderer::DrawLine(start, end, color);
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

			Renderer::DrawLine(planeScreenVertexes[0], planeScreenVertexes[1], color);
			Renderer::DrawLine(planeScreenVertexes[1], planeScreenVertexes[2], color);
			Renderer::DrawLine(planeScreenVertexes[2], planeScreenVertexes[3], color);
			Renderer::DrawLine(planeScreenVertexes[3], planeScreenVertexes[0], color);

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
