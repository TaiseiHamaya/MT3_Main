#pragma once
#include <Novice.h>

#include "Vector2D.h"
#include "Vector3D.h"

class Renderer final {
public:
	/// <summary>
	/// 長方形描画
	/// <param name="pos">描画する四角形の位置</param>
	/// <param name="radius">描画する四角形の半径</param>
	/// <param name="color">描画する四角形の色</param>
	/// <param name="entity">塗りつぶしモード</param>
	/// </summary>
	static void DrawBox(
		const Vector2& pos, const Vector2& radius, const unsigned int color,
		const FillMode fillMode
	);

	/// <summary>
	/// 楕円描画
	/// <param name="pos">描画する楕円の位置</param>
	/// <param name="radius">描画する楕円の半径</param>
	/// <param name="color">描画する楕円の色</param>
	/// <param name="entity">塗りつぶしモード</param>
	/// </summary>
	static void DrawEllipse(
		const Vector2& pos, const Vector2& radius, const unsigned int color,
		const FillMode fillMode
	);

	/// <summary>
	/// 円形ゲージを描画する
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="radius"></param>
	/// <param name="percentage"></param>
	/// <param name="color"></param>
	/// <param name="spritehandle"></param>
	static void DrawCircleGauge(
		const Vector2 pos, const Vector2 radius, const float percentage, const unsigned int color,
		const int spritehandle
	);

	/// <summary>
	/// 線を描画
	/// </summary>
	/// <param name="initial">始点</param>
	/// <param name="terminal">終点</param>
	/// <param name="color">色</param>
	static void DrawLine(const Vector2& initial, const Vector2& terminal, unsigned int color);
	
	static void DrawLine(const Vector3& initial, const Vector3& terminal, unsigned int color);

	/// <summary>
	/// 線を描画
	/// </summary>
	/// <param name="initial">始点</param>
	/// <param name="terminal">終点</param>
	/// <param name="width">先の太さ</param>
	/// <param name="color">色</param>
	static void DrawLineWidth(const Vector2& initial, const Vector2& terminal, float width, unsigned int color);
};