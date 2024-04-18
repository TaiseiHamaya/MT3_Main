#include <Novice.h>

#include <Vector3D.h>
#include <Debug.h>

const char kWindowTitle[] = "LE2A_14_ハマヤ_タイセイ_MT3";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// -------------------------------------------ライブラリの初期化------------------------------------------
	Novice::Initialize(kWindowTitle, 1280, 720);

	// --------------------------------------------変数宣言--------------------------------------------
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Vector3 vec1{ 1.0f, 3.0f, -5.0f };
	Vector3 vec2{ 4.0f, -1.0f, 2.0f };
	float k = 4;

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

		Vector3 resultAdd = vec1 + vec2;
		Vector3 resultSub = vec1 - vec2;
		Vector3 resultMulti = vec1 * k;
		float resultDot = Vector3::DotProduct(vec1, vec2);
		float resultLength = vec1.length();
		Vector3 resultNormalize = vec2.normalize();

		///
		/// ----------------------------------------更新処理ここまで----------------------------------------
		///

		///
		/// ----------------------------------------描画処理ここから----------------------------------------
		///

		Debug::PrintVec3(0, 0, resultAdd);
		Debug::PrintVec3(0, 20, resultSub);
		Debug::PrintVec3(0, 40, resultMulti);
		Novice::ScreenPrintf(0, 60, "%f", resultDot);
		Novice::ScreenPrintf(0, 80, "%f", resultLength);
		Debug::PrintVec3(0, 100, resultNormalize);

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
