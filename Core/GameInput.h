#pragma once
#include <array>

#include "Vector2D.h"
#include "Definition.h"
#include "DoubleFlag.h"

enum MouseButton {
	MouseLeft,
	MouseRight,
	MouseMid
};

class GameInput {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize();

	/// <summary>
	/// 終了処理
	/// </summary>
	static void Finalize();

	/// <summary>
	/// キー入力のアップデート
	/// </summary>
	static void Update();

	/// <summary>
	/// キー入力の受け渡し
	/// </summary>
	/// <returns></returns>
	static char* GetKeys();

	/// <summary>
	/// 1フレーム前のキー入力の受け渡し
	/// </summary>
	/// <returns></returns>
	static char* GetPreKeys();

	/// <summary>
	/// そのキーが押された瞬間かどうか
	/// </summary>
	/// <param name="key">キー番号</param>
	/// <returns></returns>
	static bool IsTriggerKey(int key);

	/// <summary>
	/// そのキーが押されている最中かどうか
	/// </summary>
	/// <param name="key">キー番号</param>
	/// <returns></returns>
	static bool IsPressKey(int key);

	/// <summary>
	/// そのキーが離れたかどうか
	/// </summary>
	/// <param name="key">キー番号</param>
	/// <returns></returns>
	static bool IsReleaseKey(int key);

	static bool IsTriggerMouse(MouseButton button);
	static bool IsPressMouse(MouseButton button);
	static bool IsReleaseMouse(MouseButton button);
	static const DoubleFlag& MouseState(MouseButton button);

	/// <summary>
	/// WASD入力を取得
	/// </summary>
	/// <returns></returns>
	static Vector2 InputWASD(void);

	/// <summary>
	/// WASD入力を取得
	/// <param name="entity">入力を記録するベクトル</param>
	/// </summary>
	static void InputWASD(Vector2* const inputVelocity);

	/// <summary>
	/// AD入力を取得
	/// </summary>
	/// <returns></returns>
	static float InputAD(void);

	/// <summary>
	/// AD入力を取得
	/// </summary>
	/// <param name="inputVelocityX"></param>
	static void InputAD(float* const inputVelocityX);

	/// <summary>
	/// 矢印キーによる入力
	/// </summary>
	/// <returns></returns>
	static Vector2 InputArrow(void);

	/// <summary>
	/// 矢印キーによる入力
	/// </summary>
	/// <param name="inputVelocity"></param>
	static void InputArrow(Vector2* const inputVelocity);

	/// <summary>
	/// マウスのワールド座標を取得
	/// </summary>
	/// <returns></returns>
	static Vector2 GetMousePos(void);

	static Vector2 InputLeftStick(void);

	static void InputLeftStick(Vector2* inputVelocity);

	static void SetStickDiedZone(float length);

private:
	static std::array<char, 256> keys;
	static std::array<char, 256> preKeys;
	static std::array<DoubleFlag, 3> mouseState;
	static float stickDiedZone;
};