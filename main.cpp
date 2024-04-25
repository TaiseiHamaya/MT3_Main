#include <Novice.h>

#include <Camera3D.h>
#include <DeviceData.h>

#include <Quaternion.h>
#include <Vector3D.h>
#include <Matrix.h>
#include <Debug.h>

const char kWindowTitle[] = "LE2A_14_ハマヤ_タイセイ_MT3";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// -------------------------------------------ライブラリの初期化------------------------------------------
	Novice::Initialize(kWindowTitle, 1280, 720);

	DeviceData::Initialize(1280, 720);
	Camera3D::Initialize();

	// --------------------------------------------変数宣言--------------------------------------------
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Vector3 rotate{ 0.4f, 1.43f, -0.8f };
	Matrix4x4 rotateXMatrix = Quaternion{ rotate.x, 0,0, }.to_matrix();
	Matrix4x4 rotateYMatrix = Quaternion{ 0, rotate.y,0, }.to_matrix();
	Matrix4x4 rotateZMatrix = Quaternion{ 0, 0,rotate.z, }.to_matrix();

	Matrix4x4 rotateXYZMatrix = rotateXMatrix * rotateYMatrix * rotateZMatrix;

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



		///
		/// ----------------------------------------更新処理ここまで----------------------------------------
		///

		///
		/// ----------------------------------------描画処理ここから----------------------------------------
		///

		Debug::PrintMatrix4x4(0, 0, rotateXMatrix);
		Debug::PrintMatrix4x4(0, 100, rotateYMatrix);
		Debug::PrintMatrix4x4(0, 200, rotateZMatrix);
		Debug::PrintMatrix4x4(0, 300, rotateXYZMatrix);

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
