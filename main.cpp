#include <Novice.h>

#include <Matrix.h>
#include <Vector3D.h>
#include <Debug.h>
#include <Transform3D.h>

const char kWindowTitle[] = "LE2A_14_ハマヤ_タイセイ_MT3";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// -------------------------------------------ライブラリの初期化------------------------------------------
	Novice::Initialize(kWindowTitle, 1280, 720);

	// --------------------------------------------変数宣言--------------------------------------------
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Vector3 translate{ 4.1f, 2.6f, 0.8f };
	Vector3 scale{ 1.5f, 5.2f, 7.3f };
	Matrix4x4 translateMatrix = Transform3D::MakeTranslateMatrix(translate);
	Matrix4x4 scaleMatrix = Transform3D::MakeScaleMatrix(scale);
	Vector3 point{ 2.3f, 3.8f, 1.4f };
	Matrix4x4 transformMatrix = {
		{1.0f, 2.0f, 3.0f, 4.0f},
		{3.0f, 1.0f, 1.0f, 2.0f },
		{1.0f, 4.0f, 2.0f, 3.0f},
		{2.0f, 2.0f, 1.0f, 3.0f}
	};
	Vector3 transformed = Transform3D::Homogeneous(point, transformMatrix); 
	const int kRowHeight = 20;

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


		Debug::PrintVec3(0, 0, transformed);
		Debug::PrintMatrix4x4(0, kRowHeight  * 2, translateMatrix);
		Debug::PrintMatrix4x4(0, kRowHeight * 7, scaleMatrix);

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
