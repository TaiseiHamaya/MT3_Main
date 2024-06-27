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

bool SeparatingAxisCollision(const Vector3& separatingAxis, const std::vector<Vector3>& convoxVertex1, const std::vector<Vector3>& convoxVertex2) {
	std::vector<float> projectionVertexLength1;
	projectionVertexLength1.reserve(convoxVertex1.size());
	for (int i = 0; i < convoxVertex1.size(); ++i) {
		projectionVertexLength1.emplace_back(Vector3::DotProduct(convoxVertex1[i], separatingAxis) / separatingAxis.length());
	}

	std::vector<float> projectionVertexLength2;
	projectionVertexLength2.reserve(convoxVertex2.size());
	for (int i = 0; i < convoxVertex2.size(); ++i) {
		projectionVertexLength2.emplace_back(Vector3::DotProduct(convoxVertex2[i], separatingAxis) / separatingAxis.length());
	}

	auto&& convoxMinMax1 = std::minmax_element(projectionVertexLength1.begin(), projectionVertexLength1.end());
	auto&& convoxMinMax2 = std::minmax_element(projectionVertexLength2.begin(), projectionVertexLength2.end());

	float sumSpan = *convoxMinMax1.second - *convoxMinMax1.first + *convoxMinMax2.second - *convoxMinMax2.first;
	float longSpan = (std::max)(*convoxMinMax1.second, *convoxMinMax2.second) - (std::min)(*convoxMinMax1.first, *convoxMinMax2.first);
	if (sumSpan < longSpan) {
		return true;
	}
	return false;
}

bool IsCollision(const OBB& obb1, const OBB& obb2) {
	const Matrix4x4& obb1Matrix = obb1.transfrom.get_matrix();
	// OBB頂点の取得
	std::vector<Vector3> convoxVertex1{
		Transform3D::Homogeneous(obb1.localVertexis.min, obb1Matrix),
		Transform3D::Homogeneous({obb1.localVertexis.min.x,obb1.localVertexis.min.y,obb1.localVertexis.max.z}, obb1Matrix),
		Transform3D::Homogeneous({obb1.localVertexis.min.x,obb1.localVertexis.max.y,obb1.localVertexis.max.z}, obb1Matrix),
		Transform3D::Homogeneous({obb1.localVertexis.max.x,obb1.localVertexis.min.y,obb1.localVertexis.max.z}, obb1Matrix),
		Transform3D::Homogeneous({obb1.localVertexis.min.x,obb1.localVertexis.max.y,obb1.localVertexis.min.z}, obb1Matrix),
		Transform3D::Homogeneous({obb1.localVertexis.max.x,obb1.localVertexis.min.y,obb1.localVertexis.min.z}, obb1Matrix),
		Transform3D::Homogeneous({obb1.localVertexis.max.x,obb1.localVertexis.max.y,obb1.localVertexis.min.z}, obb1Matrix),
		Transform3D::Homogeneous(obb1.localVertexis.max, obb1Matrix)
	};
	const Matrix4x4& obb2Matrix = obb2.transfrom.get_matrix();
	std::vector<Vector3> convoxVertex2{
		Transform3D::Homogeneous(obb2.localVertexis.min, obb2Matrix),
		Transform3D::Homogeneous({obb2.localVertexis.min.x,obb2.localVertexis.min.y,obb2.localVertexis.max.z}, obb2Matrix),
		Transform3D::Homogeneous({obb2.localVertexis.min.x,obb2.localVertexis.max.y,obb2.localVertexis.max.z}, obb2Matrix),
		Transform3D::Homogeneous({obb2.localVertexis.max.x,obb2.localVertexis.min.y,obb2.localVertexis.max.z}, obb2Matrix),
		Transform3D::Homogeneous({obb2.localVertexis.min.x,obb2.localVertexis.max.y,obb2.localVertexis.min.z}, obb2Matrix),
		Transform3D::Homogeneous({obb2.localVertexis.max.x,obb2.localVertexis.min.y,obb2.localVertexis.min.z}, obb2Matrix),
		Transform3D::Homogeneous({obb2.localVertexis.max.x,obb2.localVertexis.max.y,obb2.localVertexis.min.z}, obb2Matrix),
		Transform3D::Homogeneous(obb2.localVertexis.max, obb2Matrix)
	};
	// 面法線タイム
	Vector3 separatingAxis1X = Transform3D::HomogeneousVector(Vec3::kBasisX, obb1.transfrom.get_matrix());
	if (SeparatingAxisCollision(separatingAxis1X, convoxVertex1, convoxVertex2)) {
		return false;
	}
	Vector3 separatingAxis1Y = Transform3D::HomogeneousVector(Vec3::kBasisY, obb1.transfrom.get_matrix());
	if (SeparatingAxisCollision(separatingAxis1Y, convoxVertex1, convoxVertex2)) {
		return false;
	}
	Vector3 separatingAxis1Z = Transform3D::HomogeneousVector(Vec3::kBasisZ, obb1.transfrom.get_matrix());
	if (SeparatingAxisCollision(separatingAxis1Z, convoxVertex1, convoxVertex2)) {
		return false;
	}
	Vector3 separatingAxis2X = Transform3D::HomogeneousVector(Vec3::kBasisX, obb2.transfrom.get_matrix());
	if (SeparatingAxisCollision(separatingAxis2X, convoxVertex1, convoxVertex2)) {
		return false;
	}
	Vector3 separatingAxis2Y = Transform3D::HomogeneousVector(Vec3::kBasisY, obb2.transfrom.get_matrix());
	if (SeparatingAxisCollision(separatingAxis2Y, convoxVertex1, convoxVertex2)) {
		return false;
	}
	Vector3 separatingAxis2Z = Transform3D::HomogeneousVector(Vec3::kBasisZ, obb2.transfrom.get_matrix());
	if (SeparatingAxisCollision(separatingAxis2Z, convoxVertex1, convoxVertex2)) {
		return false;
	}

	// クロス積タイム
	if (SeparatingAxisCollision(Vector3::CrossProduct(separatingAxis1X, separatingAxis2X), convoxVertex1, convoxVertex2)) {
		return false;
	}
	if (SeparatingAxisCollision(Vector3::CrossProduct(separatingAxis1X, separatingAxis2Y), convoxVertex1, convoxVertex2)) {
		return false;
	}
	if (SeparatingAxisCollision(Vector3::CrossProduct(separatingAxis1X, separatingAxis2Z), convoxVertex1, convoxVertex2)) {
		return false;
	}
	if (SeparatingAxisCollision(Vector3::CrossProduct(separatingAxis1Y, separatingAxis2X), convoxVertex1, convoxVertex2)) {
		return false;
	}
	if (SeparatingAxisCollision(Vector3::CrossProduct(separatingAxis1Y, separatingAxis2Y), convoxVertex1, convoxVertex2)) {
		return false;
	}
	if (SeparatingAxisCollision(Vector3::CrossProduct(separatingAxis1Y, separatingAxis2Z), convoxVertex1, convoxVertex2)) {
		return false;
	}
	if (SeparatingAxisCollision(Vector3::CrossProduct(separatingAxis1Z, separatingAxis2X), convoxVertex1, convoxVertex2)) {
		return false;
	}
	if (SeparatingAxisCollision(Vector3::CrossProduct(separatingAxis1Z, separatingAxis2Y), convoxVertex1, convoxVertex2)) {
		return false;
	}
	if (SeparatingAxisCollision(Vector3::CrossProduct(separatingAxis1Z, separatingAxis2Z), convoxVertex1, convoxVertex2)) {
		return false;
	}
	return true;
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
	OBB obb1{
		{ {-0.5f,-0.5f,-0.5f}, {0.5f,0.5f,0.5f} },
		{  }
	};
	OBB obb2{
		{ {-0.5f,-0.5f,-0.5f}, {0.5f,0.5f,0.5f} },
		{ Vec3::kBasis, Quaternion{-0.05f, -2.49f, 0.15f}, Vector3{0.9f,0.66f, 0.78f } }
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

		{	// OBB1
			ImGui::SetNextWindowPos({ 959, 125 }, ImGuiCond_Once);
			ImGui::SetNextWindowSize({ 298, 173 }, ImGuiCond_Once);
			ImGui::Begin("OBB1", nullptr, ImGuiWindowFlags_NoSavedSettings);
			ImGui::DragFloat3("Min", &obb1.localVertexis.min.x, 0.1f);
			ImGui::DragFloat3("Max", &obb1.localVertexis.max.x, 0.1f);
			ImGui::Separator();
			if (ImGui::Button("ResetRotate")) {
				obb1.transfrom.get_quaternion() = Quaternion{};
			}
			Vector3 quaternion = Vec3::kZero;
			if (ImGui::DragFloat3("RotateLocal", &quaternion.x, 0.01f, -PI, PI)) {
				obb1.transfrom.get_quaternion() = Quaternion{ quaternion } *obb1.transfrom.get_quaternion();
			}
			Vector3 cood = Vec3::kZero;
			if (ImGui::DragFloat3("RotateWorld", &cood.x, 0.02f)) {
				obb1.transfrom.get_quaternion() *= Quaternion{ cood, cood.length() };
			}
			ImGui::DragFloat3("Translate", const_cast<float*>(&obb1.transfrom.get_translate().x), 0.1f);
			{
				auto boxResultX = std::minmax(obb1.localVertexis.min.x, obb1.localVertexis.max.x);
				auto boxResultY = std::minmax(obb1.localVertexis.min.y, obb1.localVertexis.max.y);
				auto boxResultZ = std::minmax(obb1.localVertexis.min.z, obb1.localVertexis.max.z);
				obb1.localVertexis.min = { boxResultX.first, boxResultY.first, boxResultZ.first };
				obb1.localVertexis.max = { boxResultX.second, boxResultY.second, boxResultZ.second };
			}
			ImGui::End();
		}

		{	// OBB2
			ImGui::SetNextWindowPos({ 959, 325 }, ImGuiCond_Once);
			ImGui::SetNextWindowSize({ 298, 173 }, ImGuiCond_Once);
			ImGui::Begin("OBB2", nullptr, ImGuiWindowFlags_NoSavedSettings);
			ImGui::DragFloat3("Min", &obb2.localVertexis.min.x, 0.1f);
			ImGui::DragFloat3("Max", &obb2.localVertexis.max.x, 0.1f);
			ImGui::Separator();
			if (ImGui::Button("ResetRotate")) {
				obb2.transfrom.get_quaternion() = Quaternion{};
			}
			Vector3 quaternion = Vec3::kZero;
			if (ImGui::DragFloat3("RotateLocal", &quaternion.x, 0.01f, -PI, PI)) {
				obb2.transfrom.get_quaternion() = Quaternion{ quaternion } *obb2.transfrom.get_quaternion();
			}
			Vector3 cood = Vec3::kZero;
			if (ImGui::DragFloat3("RotateWorld", &cood.x, 0.02f)) {
				obb2.transfrom.get_quaternion() *= Quaternion{ cood, cood.length() };
			}
			ImGui::DragFloat3("Translate", const_cast<float*>(&obb2.transfrom.get_translate().x), 0.1f);
			{
				auto boxResultX = std::minmax(obb2.localVertexis.min.x, obb2.localVertexis.max.x);
				auto boxResultY = std::minmax(obb2.localVertexis.min.y, obb2.localVertexis.max.y);
				auto boxResultZ = std::minmax(obb2.localVertexis.min.z, obb2.localVertexis.max.z);
				obb2.localVertexis.min = { boxResultX.first, boxResultY.first, boxResultZ.first };
				obb2.localVertexis.max = { boxResultX.second, boxResultY.second, boxResultZ.second };
			}
			ImGui::End();
		}

		obb1.transfrom.update();
		obb2.transfrom.update();

		if (IsCollision(obb1, obb2)) {
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
		DrawOBB(obb1, hexColor);
		DrawOBB(obb2, hexColor);

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
