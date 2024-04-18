#pragma once

class Vector2;
class Vector3;
class Matrix3x3;
class Matrix4x4;

class Debug {
public:
	static void BlackBakcground();
	static void Grid2D();
	static void Grid3D();
	static void PrintVec2(int x, int y, const Vector2& vec);
	static void PrintVec3(int x, int y, const Vector3& vec);
	static void PrintMatrix3x3(int x, int y, const Matrix3x3& matrix);
	static void PrintMatrix4x4(int x, int y, const Matrix4x4& matrix);
};