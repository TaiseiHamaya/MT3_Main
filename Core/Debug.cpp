#include "Debug.h"

#include <cmath>
#include <Novice.h>

#include "DeviceData.h"
#include "Easing.h"
#include "Camera3D.h"
#include "Renderer.h"
#include "Transform2D.h"
#include "Vector3D.h"
#include "Vector2D.h"

void Debug::BlackBakcground() {
	//static Vector2 topLeft = Transform2D::Homogeneous({ -1,1 }, Camera2D::instance->viewportMatrix);
	//static Vector2 bottomRight = Transform2D::Homogeneous({ 1,-1 }, Camera2D::instance->viewportMatrix);
	//Novice::DrawBox(int(topLeft.x), int(topLeft.y), int(bottomRight.x - topLeft.x), int(bottomRight.y - topLeft.y), 0, BLACK, kFillModeSolid);
}

void Debug::Grid2D() {
	//static constexpr float gridHalfLength = 2000;
	//static constexpr float gridDistance = 100;

	//Vector2 mid = Camera2D::instance->camera.get_translate() / gridDistance;
	//std::modf(mid.x, &mid.x);
	//std::modf(mid.y, &mid.y);
	//mid *= gridDistance;

	//Renderer::DrawLine(Transform2D::Homogeneous(mid + Vector2{ 0,gridHalfLength }, Camera2D::GetVPVMatrix()), Transform2D::Homogeneous(mid + Vector2{ 0,-gridHalfLength }, Camera2D::GetVPVMatrix()), 0xFFFFFF30);
	//Renderer::DrawLine(Transform2D::Homogeneous(mid + Vector2{ gridHalfLength,0 }, Camera2D::GetVPVMatrix()), Transform2D::Homogeneous(mid + Vector2{ -gridHalfLength,0 }, Camera2D::GetVPVMatrix()), 0xFFFFFF30);
	//for (int i = 1; i <= 20; ++i) {
	//	Renderer::DrawLine(Transform2D::Homogeneous(mid + Vector2{ gridDistance * i, gridHalfLength }, Camera2D::GetVPVMatrix()), Transform2D::Homogeneous(mid + Vector2{ gridDistance * i, -gridHalfLength }, Camera2D::GetVPVMatrix()), 0xFFFFFF30);
	//	Renderer::DrawLine(Transform2D::Homogeneous(mid + Vector2{ -gridDistance * i, gridHalfLength }, Camera2D::GetVPVMatrix()), Transform2D::Homogeneous(mid + Vector2{ -gridDistance * i, -gridHalfLength }, Camera2D::GetVPVMatrix()), 0xFFFFFF30);

	//	Renderer::DrawLine(Transform2D::Homogeneous(mid + Vector2{ gridHalfLength, gridDistance * i }, Camera2D::GetVPVMatrix()), Transform2D::Homogeneous(mid + Vector2{ -gridHalfLength, gridDistance * i }, Camera2D::GetVPVMatrix()), 0xFFFFFF30);
	//	Renderer::DrawLine(Transform2D::Homogeneous(mid + Vector2{ gridHalfLength, -gridDistance * i }, Camera2D::GetVPVMatrix()), Transform2D::Homogeneous(mid + Vector2{ -gridHalfLength, -gridDistance * i }, Camera2D::GetVPVMatrix()), 0xFFFFFF30);
	//}

	//Vector2 zeroBottom = Transform2D::Homogeneous(Vector2{ 0, gridHalfLength }, Camera2D::GetVPVMatrix());
	//Vector2 zeroTop = Transform2D::Homogeneous(Vector2{ 0,-gridHalfLength }, Camera2D::GetVPVMatrix());
	//Vector2 zeroLeft = Transform2D::Homogeneous(Vector2{ gridHalfLength,0 }, Camera2D::GetVPVMatrix());
	//Vector2 zeroRight = Transform2D::Homogeneous(Vector2{ -gridHalfLength,0 }, Camera2D::GetVPVMatrix());

	//Renderer::DrawLineWidth(zeroBottom, zeroTop, 3, RED);
	//Renderer::DrawLineWidth(zeroLeft, zeroRight, 3, BLUE);
}

void Debug::Grid3D() {
	static constexpr float gridDistance = 1;
	static constexpr float gridTimes = 10;
	static constexpr float gridHalfLength = gridDistance * gridTimes;

	Vector3 cameraRay = Transform3D::HomogeneousVector(Vec3::kBasisZ, Camera3D::GetCameraTransform().get_quaternion().to_matrix());
	float dot = Vector3::DotProduct(cameraRay, Vec3::kBasisY);
	if (dot == 0) {
		return;
	}
	Vector3 cameraPos = Camera3D::GetCameraTransform().get_translate();
	float t = (0 - Vector3::DotProduct(cameraPos, Vec3::kBasisY)) / dot;
	if (t <= 0) {
		return;
	}

	Vector3 target = (cameraPos + (cameraRay * t));

	Vector3 mid = target / gridDistance;
	std::modf(mid.x, &mid.x);
	mid.y = 0;
	std::modf(mid.z, &mid.z);
	mid *= gridDistance;

	Renderer::DrawLine(Transform3D::Homogeneous(mid + Vector3{ 0, 0,gridHalfLength }, Camera3D::GetVPOVMatrix()), Transform3D::Homogeneous(mid + Vector3{ 0, 0,-gridHalfLength }, Camera3D::GetVPOVMatrix()), 0xFFFFFF30);
	Renderer::DrawLine(Transform3D::Homogeneous(mid + Vector3{ gridHalfLength,0,0 }, Camera3D::GetVPOVMatrix()), Transform3D::Homogeneous(mid + Vector3{ -gridHalfLength,0, 0 }, Camera3D::GetVPOVMatrix()), 0xFFFFFF30);
	for (int i = 1; i <= gridTimes; ++i) {
		Renderer::DrawLine(Transform3D::Homogeneous(mid + Vector3{ gridDistance * i, 0, gridHalfLength }, Camera3D::GetVPOVMatrix()), Transform3D::Homogeneous(mid + Vector3{ gridDistance * i, 0, -gridHalfLength }, Camera3D::GetVPOVMatrix()), 0xFFFFFF30);
		Renderer::DrawLine(Transform3D::Homogeneous(mid + Vector3{ -gridDistance * i, 0, gridHalfLength }, Camera3D::GetVPOVMatrix()), Transform3D::Homogeneous(mid + Vector3{ -gridDistance * i, 0, -gridHalfLength }, Camera3D::GetVPOVMatrix()), 0xFFFFFF30);

		Renderer::DrawLine(Transform3D::Homogeneous(mid + Vector3{ gridHalfLength, 0, gridDistance * i }, Camera3D::GetVPOVMatrix()), Transform3D::Homogeneous(mid + Vector3{ -gridHalfLength, 0, gridDistance * i }, Camera3D::GetVPOVMatrix()), 0xFFFFFF30);
		Renderer::DrawLine(Transform3D::Homogeneous(mid + Vector3{ gridHalfLength, 0, -gridDistance * i }, Camera3D::GetVPOVMatrix()), Transform3D::Homogeneous(mid + Vector3{ -gridHalfLength, 0, -gridDistance * i }, Camera3D::GetVPOVMatrix()), 0xFFFFFF30);
	}

	Vector3 zeroBottom = Transform3D::Homogeneous(Vector3{ 0, 0, mid.z + gridHalfLength }, Camera3D::GetVPOVMatrix());
	Vector3 zeroTop = Transform3D::Homogeneous(Vector3{ 0, 0,mid.z - gridHalfLength }, Camera3D::GetVPOVMatrix());
	Vector3 zeroLeft = Transform3D::Homogeneous(Vector3{ mid.x + gridHalfLength, 0, 0 }, Camera3D::GetVPOVMatrix());
	Vector3 zeroRight = Transform3D::Homogeneous(Vector3{ mid.x - gridHalfLength, 0, 0 }, Camera3D::GetVPOVMatrix());

	Renderer::DrawLine(zeroLeft, zeroRight, RED);
	Renderer::DrawLine(zeroBottom, zeroTop, GREEN);
}

void Debug::PrintVec2(int x, int y, const Vector2& vec) {
	Novice::ScreenPrintf(x, y, "% 6.2f % 6.2f", vec.x, vec.y);
}

void Debug::PrintVec3(int x, int y, const Vector3& vec) {
	Novice::ScreenPrintf(x, y, "% 6.2f % 6.2f % 6.2f", vec.x, vec.y, vec.z);
}

void Debug::PrintMatrix3x3(int x, int y, const Matrix3x3& matrix) {
	static const int kRowHeight = 20;
	static const int kColumnWidth = 80;
	for (int row = 0; row < 3; ++row) {
		for (int column = 0; column < 3; ++column) {
			Novice::ScreenPrintf(x + column * kColumnWidth, y + row * kRowHeight, "% 4.2f", matrix[row][column]);
		}
	}
}

void Debug::PrintMatrix4x4(int x, int y, const Matrix4x4& matrix) {
	static const int kRowHeight = 20;
	static const int kColumnWidth = 60;
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(x + column * kColumnWidth, y + row * kRowHeight, "% 4.2f", matrix[row][column]);
		}
	}
}
