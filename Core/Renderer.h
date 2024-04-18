#pragma once
#include <Novice.h>
#include <string>

#include "Vector2D.h"
#include "Matrix.h"
#include "Transform2D.h"
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
	/// スプライトの描画
	/// </summary>
	/// <param name="rect">ローカル頂点</param>
	/// <param name="spriteSize">スプライトのWH(Vector2)</param>
	/// <param name="textureName">テクスチャ名</param>
	/// <param name="move">ワールド座標変換行列</param>
	/// <param name="camera">スクリーン座標変換行列</param>
	/// <param name="color">色</param>
	static void DrawSprite(
		const Rect& rect, const Vector2& spriteSize, const std::string& textureName, const Matrix3x3& worldMatrix, const Matrix3x3& worldToScreenMatrix = Transform2D::MakeIdentityMatrix(),
		const unsigned int color = WHITE
	);

	/// <summary>
	/// アニメーション付きビットマップを描画
	/// </summary>
	/// <param name="rect">ローカル頂点</param>
	/// <param name="spriteSize">1ビットマップのWH(Vector2)</param>
	/// <param name="textureName">テクスチャ名</param>
	/// <param name="frame">アニメーションフレーム数(row)</param>
	/// <param name="move">ワールド座標変換行列</param>
	/// <param name="camera">スクリーン座標変換行列</param>
	/// <param name="color">色</param>
	static void DrawSpriteAnimation(
		const Rect& rect, const Vector2& spriteSize, const std::string& textureName, const int frame, const Matrix3x3& move, const Matrix3x3& camera = Transform2D::MakeIdentityMatrix(),
		const unsigned int color = WHITE
	);

	/// <summary>
	/// アニメーション付きビットマップを描画
	/// </summary>
	/// <param name="rect">ローカル頂点</param>
	/// <param name="spriteSize">1ビットマップのWH(Vector2)</param>
	/// <param name="textureName">テクスチャ名</param>
	/// <param name="bitmapNum">連番画像(column)</param>
	/// <param name="frame">アニメーションフレーム数(row)</param>
	/// <param name="move">ワールド座標変換行列</param>
	/// <param name="camera">スクリーン座標変換行列</param>
	/// <param name="color">色</param>
	static void DrawBitmapAnimation(
		const Rect& rect, const Vector2& spriteSize, const std::string& textureName, const int bitmapNum, const int frame, const Matrix3x3& worldMatrix, const Matrix3x3& worldToScreenMatrix = Transform2D::MakeScaleMatrix(1, 1),
		const unsigned int color = WHITE
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

	static void DrawUI(const Rect& rect, const Vector2& spriteSize, const std::string& textureName, const int frame);

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