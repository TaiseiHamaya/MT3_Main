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

struct Triangle {
	std::array<Vector3, 3> vertices;
};

bool IsCollision(const Plane& plane, const Segment& segment) {
	float dot = Vector3::DotProduct(plane.normal, segment.diff);

	if (dot == 0) {
		return false;
	}
	float t = (plane.distance - Vector3::DotProduct(plane.normal, segment.origin)) / dot;

	return t >= 0 && t <= 1;
}

bool IsCollision(const Triangle& triangle, const Segment& segment) {
	Plane trianglePlane;
	trianglePlane.normal = Vector3::CrossProduct(triangle.vertices[1] - triangle.vertices[0], triangle.vertices[2] - triangle.vertices[1]).normalize();
	trianglePlane.distance = Vector3::DotProduct(triangle.vertices[0], trianglePlane.normal);

	if (!IsCollision(trianglePlane, segment)) {
		return false;
	}
	float dot = Vector3::DotProduct(trianglePlane.normal, segment.diff);
	float t = (trianglePlane.distance - Vector3::DotProduct(trianglePlane.normal, segment.origin)) / dot;
	Vector3 collisionPoint = segment.origin + segment.diff * t;

	Vector3 cross1 = Vector3::CrossProduct(triangle.vertices[1] - triangle.vertices[0], collisionPoint - triangle.vertices[1]);
	Vector3 cross2 = Vector3::CrossProduct(triangle.vertices[2] - triangle.vertices[1], collisionPoint - triangle.vertices[2]);
	Vector3 cross3 = Vector3::CrossProduct(triangle.vertices[0] - triangle.vertices[2], collisionPoint - triangle.vertices[0]);

	if (
		Vector3::DotProduct(trianglePlane.normal, cross1) >= 0 &&
		Vector3::DotProduct(trianglePlane.normal, cross2) >= 0 &&
		Vector3::DotProduct(trianglePlane.normal, cross3) >= 0
		) {
		return true;
	}
	return false;
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
	Color segmentColor = { 1.0f,1.0f,1.0f,1.0f };
	Triangle triangel = {
		{Vec3::kBasisX,Vec3::kBasisY,Vec3::kBasisZ }
	};


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


		// triangle
		ImGui::SetNextWindowSize(ImVec2{ 300,210 }, ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2{ 900, 350 }, ImGuiCond_Once);
		ImGui::Begin("Triangel", nullptr, ImGuiWindowFlags_NoSavedSettings);
		ImGui::DragFloat3("Position0", &triangel.vertices[0].x, 0.1f);
		ImGui::DragFloat3("Position1", &triangel.vertices[1].x, 0.1f);
		ImGui::DragFloat3("Position2", &triangel.vertices[2].x, 0.1f);
		ImGui::End();

		if (IsCollision(triangel, segment)) {
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

		{ // draw triangle
			Triangle screenTriangle;
			for (int i = 0; i < screenTriangle.vertices.size(); ++i) {
				screenTriangle.vertices[i] = Transform3D::Homogeneous(triangel.vertices[i], Camera3D::GetVPOVMatrix());
			}
			Renderer::DrawLine(screenTriangle.vertices[0], screenTriangle.vertices[1], color);
			Renderer::DrawLine(screenTriangle.vertices[1], screenTriangle.vertices[2], color);
			Renderer::DrawLine(screenTriangle.vertices[2], screenTriangle.vertices[0], color);
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
