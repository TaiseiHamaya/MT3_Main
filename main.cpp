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

struct Segment {
public:
	Vector3 origin;
	Vector3 diff;
};

static Vector3 ClosestPoint(const Vector3& point, const Segment& segment) {
	float segmentLength = segment.diff.length();
	float t = Vector3::DotProduct(point - segment.origin, segment.diff) / (segmentLength * segmentLength);
	t = std::clamp(t, 0.f, 1.0f);
	return segment.diff * t + segment.origin;
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

	Sphere point{ Transform3D{ Vec3::kBasis, Vec3::kZero, {-1.5f, 0.6f, 0.6f }}, Color{ 1.0f, 0.0f, 0.0f, 1.0f }, 0.01f, 16 };
	Sphere closestPoint{ Transform3D{ Vec3::kBasis, Vec3::kZero, Vec3::kZero }, Color{ 0.0f, 0.0f, 0.0f, 1.0f }, 0.01f, 16 };
	Segment segment{ { -2.0f, -1.0f, 0.0f },{ 3.0f, 2.0f, 2.0f } };

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

		// 点
		point.begin();
		ImGui::SetNextWindowSize(ImVec2{ 300,210 }, ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2{ 900, 100 }, ImGuiCond_Once);
		ImGui::Begin("Sphere", nullptr, ImGuiWindowFlags_NoSavedSettings);
		point.debug_gui();
		ImGui::End();
		point.update();
		point.begin_rendering();

		// 線分
		ImGui::SetNextWindowSize(ImVec2{ 300,185 }, ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2{ 900, 350 }, ImGuiCond_Once);
		ImGui::Begin("Segment", nullptr, ImGuiWindowFlags_NoSavedSettings);
		ImGui::DragFloat3("Origin", &segment.origin.x, 0.1f);
		ImGui::DragFloat3("Diff", &segment.diff.x, 0.1f);
		ImGui::End();

		// 交点
		closestPoint.begin();
		closestPoint.get_transform().set_translate(
			ClosestPoint(point.get_transform().get_translate(), segment)
		);
		closestPoint.update();
		closestPoint.begin_rendering();

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
		point.draw();
		closestPoint.draw();
		Vector3 start = Transform3D::Homogeneous(segment.origin, Camera3D::GetVPOVMatrix());
		Vector3 end = Transform3D::Homogeneous(segment.origin + segment.diff, Camera3D::GetVPOVMatrix());
		Renderer::DrawLine(start, end, WHITE);

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
