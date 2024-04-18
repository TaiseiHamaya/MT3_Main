#pragma once

#include "Vector2D.h"
#include <memory>

class DeviceData final {
private:// シングルトンなのでprivate
	DeviceData() = default;
	DeviceData(float windowWidth, float windowHight);

public:
	~DeviceData() = default;
	// コピー禁止
	DeviceData(const DeviceData&) = delete;
	DeviceData& operator=(const DeviceData&) = delete;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="windowWidth">ウィンドウの幅のピクセル数</param>
	/// <param name="windowHight">ウィンドウの高さ</param>
	static void Initialize(int windowWidth, int windowHight);

	/// <summary>
	/// ゲームウィンドウの大きさを取得
	/// </summary>
	/// <returns></returns>
	static Vector2 GetWindowSize();

	/// <summary>
	/// スクリーンの大きさを取得(未定義)
	/// </summary>
	/// <returns></returns>
	static Vector2 GetScreenSize();

	//static void SetWindowMode();

	//static void Update();

	//static float ScreenTopOnWorld();
	//static float ScreenBottomOnWorld();
	//static float ScreenLeftOnWorld();
	//static float ScreenRightOnWorld();

private:// シングルトン用
	static std::unique_ptr<DeviceData> instance;

private:
	Vector2 windowSize;
	Vector2 screenSize;
	float world_screen_top;
	float world_screen_bottom;
	float world_screen_left;
	float world_screen_right;

private:
	void _Update();
};