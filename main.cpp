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

struct OBB {
	AABB localVertexis;
	Transform3D transfrom;
};

void DrawOBB(const OBB& obb, unsigned int color) {
	Matrix4x4 wvpv = obb.transfrom.get_matrix() * Camera3D::GetVPOVMatrix();
	std::array<Vector3, 8> screenPos;
	screenPos[0] = Transform3D::Homogeneous(obb.localVertexis.min, wvpv);
	screenPos[1] = Transform3D::Homogeneous({ obb.localVertexis.min.x,obb.localVertexis.max.y,obb.localVertexis.min.z }, wvpv);
	screenPos[2] = Transform3D::Homogeneous({ obb.localVertexis.max.x,obb.localVertexis.min.y,obb.localVertexis.min.z }, wvpv);
	screenPos[3] = Transform3D::Homogeneous({ obb.localVertexis.max.x,obb.localVertexis.max.y,obb.localVertexis.min.z }, wvpv);
	screenPos[4] = Transform3D::Homogeneous({ obb.localVertexis.min.x,obb.localVertexis.min.y,obb.localVertexis.max.z }, wvpv);
	screenPos[5] = Transform3D::Homogeneous({ obb.localVertexis.min.x,obb.localVertexis.max.y,obb.localVertexis.max.z }, wvpv);
	screenPos[6] = Transform3D::Homogeneous({ obb.localVertexis.max.x,obb.localVertexis.min.y,obb.localVertexis.max.z }, wvpv);
	screenPos[7] = Transform3D::Homogeneous({ obb.localVertexis.max.x,obb.localVertexis.max.y,obb.localVertexis.max.z }, wvpv);

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

bool IsCollision(const AABB& aabb, const Vector3& position, float radius) {
	Vector3 closest = Vector3::Clamp(position, aabb.min, aabb.max);
	if ((position - closest).length() <= radius) {
		return true;
	}
	return false;
}

bool IsCollision(const OBB& obb, const Sphere& sphere) {
	Vector3 sphereTranslate = Transform3D::Homogeneous(sphere.get_transform().get_translate(), obb.transfrom.get_matrix().inverse());

	return IsCollision(obb.localVertexis, sphereTranslate, sphere.get_radius());
}

const char kWindowTitle[] = "LE2A_14_ハマヤ_タイセイ_MT3";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// -------------------------------------------ライブラリの初期化------------------------------------------
	Novice::Initialize(kWindowTitle, 1280, 720);

	DeviceData::Initialize(1280, 720);
	Camera3D::Initialize();
	Camera3D::SetCameraPos({ 0,0.5f, -12.0f });
	Camera3D::CameraUpdate();

	// --------------------------------------------変数宣言--------------------------------------------
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Color color = { 1.0f,1.0f,1.0f,1.0f };
	OBB obb{
		{ {-0.5f,-0.5f,-0.5f}, {0.5f,0.5f,0.5f} },
		{  }
	};

	Sphere sphere{ {Vec3::kBasis,Quaternion::Identity(), {1.55f, 0,0}},{},1,16 };

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

		ImGui::SetNextWindowPos({ 959, 248 }, 0);
		ImGui::SetNextWindowSize({ 298, 146 }, 0);
		ImGui::Begin("Box", nullptr, ImGuiWindowFlags_NoSavedSettings);
		ImGui::DragFloat3("Min", &obb.localVertexis.min.x, 0.1f);
		ImGui::DragFloat3("Max", &obb.localVertexis.max.x, 0.1f);
		//ImGui::DragFloat3("Translate", const_cast<float*>(&obb.transfrom.get_translate().x), 0.1f);
		//obb.transfrom.get_quaternion()
		Vector3 quaternion = Vec3::kZero;
		if (ImGui::DragFloat3("RotateLocal", &quaternion.x, 0.01f, -PI, PI)) {
			obb.transfrom.get_quaternion() = Quaternion{ quaternion } *obb.transfrom.get_quaternion();
		}
		Vector3 cood = Vec3::kZero;
		if (ImGui::DragFloat3("RotateWorld", &cood.x, 0.02f)) {
			obb.transfrom.get_quaternion() *= Quaternion{ cood, cood.length() };
		}
		if (ImGui::DragFloat3("Translate", const_cast<float*>(&obb.transfrom.get_translate().x), 0.1f)) {
		}
		{
			auto boxResultX = std::minmax(obb.localVertexis.min.x, obb.localVertexis.max.x);
			auto boxResultY = std::minmax(obb.localVertexis.min.y, obb.localVertexis.max.y);
			auto boxResultZ = std::minmax(obb.localVertexis.min.z, obb.localVertexis.max.z);
			obb.localVertexis.min = { boxResultX.first, boxResultY.first, boxResultZ.first };
			obb.localVertexis.max = { boxResultX.second, boxResultY.second, boxResultZ.second };
		}
		ImGui::End();

		//ImGui::SetNextWindowSize();
		ImGui::SetNextWindowPos({ 957,24 }, 0);
		ImGui::SetNextWindowSize({ 301,209 }, 0);
		ImGui::Begin("Sphere", nullptr, ImGuiWindowFlags_NoSavedSettings);
		sphere.debug_gui();
		ImGui::End();
		sphere.update();
		sphere.begin_rendering();

		obb.transfrom.update();

		if (IsCollision(obb, sphere)) {
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
		DrawOBB(obb, hexColor);
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
