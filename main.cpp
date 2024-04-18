#include <Novice.h>

#include <Matrix.h>
#include <Debug.h>

const char kWindowTitle[] = "LE2A_14_ハマヤ_タイセイ_MT3";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// -------------------------------------------ライブラリの初期化------------------------------------------
	Novice::Initialize(kWindowTitle, 1280, 720);

	// --------------------------------------------変数宣言--------------------------------------------
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Matrix4x4 m1 = {
		{ 3.2f, 0.7f, 9.6f, 4.4f },
		{ 5.5f,1.3f, 7.8f, 2.1f },
		{ 6.9f, 8.0f, 2.6f, 1.0f },
		{ 0.5f, 7.2f, 5.1f, 3.3f  },
	};
	Matrix4x4 m2{
		{4.1f, 6.5f, 3.3f, 2.2f},
		{8.8f, 0.6f, 9.9f, 7.7f},
		{1.1f, 5.5f, 6.6f, 0.0f},
		{3.3f, 9.9f, 8.8f, 2.2f}
	};

	const int kRowHeight = 20;
	const int kColumnWidth = 60;

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

		Matrix4x4 resultAdd = m1 + m2;
		Matrix4x4 resultMultiply = m1 * m2;
		Matrix4x4 resultSubtract = m1 - m2;
		Matrix4x4 inverseM1 = m1.inverse();
		Matrix4x4 inverseM2 = Matrix4x4::Inverse(m2);
		Matrix4x4 transposeM1 = m1.transpose();
		Matrix4x4 transposeM2 = Matrix4x4::Transpose(m2);
		Matrix4x4 identity = Matrix4x4::identity;

		///
		/// ----------------------------------------更新処理ここまで----------------------------------------
		///

		///
		/// ----------------------------------------描画処理ここから----------------------------------------
		///



		Debug::PrintMatrix4x4(0, 0, resultAdd);
		Debug::PrintMatrix4x4(0, kRowHeight * 5, resultSubtract);
		Debug::PrintMatrix4x4(0, kRowHeight * 5 * 2, resultMultiply);
		Debug::PrintMatrix4x4(0, kRowHeight * 5 * 3, inverseM1);
		Debug::PrintMatrix4x4(0, kRowHeight * 5 * 4, inverseM2);
		Debug::PrintMatrix4x4(kColumnWidth * 5, 0, transposeM1);
		Debug::PrintMatrix4x4(kColumnWidth * 5, kRowHeight * 5, transposeM2);
		Debug::PrintMatrix4x4(kColumnWidth * 5, kRowHeight * 10, identity);

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
