#include "Renderer.h"

#include <vector>

#include "Matrix.h"
#include "Transform2D.h"

void Renderer::DrawBox(
	const Vector2& pos, const Vector2& radius, const unsigned int color,
	const FillMode fillMode
) {
	Novice::DrawBox(
		int(pos.x - radius.x), int(pos.y - radius.y),
		int(radius.x * 2), int(radius.y * 2), 0, color,
		fillMode
	);
}

void Renderer::DrawEllipse(
	const Vector2& pos, const Vector2& radius, const unsigned int color,
	const FillMode fillMode
) {
	Novice::DrawEllipse(
		int(pos.x), int(pos.y), int(radius.x),
		int(radius.y), 0, color, fillMode
	);
}

//void Renderer::DrawSprite(const Rect& rect, const Vector2& spriteSize, const std::string& textureName, const Matrix3x3& worldMatrix, const Matrix3x3& worldToScreenMatrix, const unsigned int color) {
//	Rect newRect = Transform2D::Homogeneous(rect, worldMatrix);
//	newRect = Transform2D::Homogeneous(newRect, worldToScreenMatrix);
//
//	Novice::DrawQuad(
//		int(newRect[3].x), int(newRect[3].y),
//		int(newRect[2].x), int(newRect[2].y),
//		int(newRect[1].x), int(newRect[1].y),
//		int(newRect[0].x), int(newRect[0].y),
//		0, 0, int(spriteSize.x), int(spriteSize.y),
//		GameTextureManager::GetHandle(textureName), color
//	);
//}
//
//void Renderer::DrawSpriteAnimation(const Rect& rect, const Vector2& spriteSize, const std::string& textureName, const int frame, const Matrix3x3& worldMatrix, const Matrix3x3& worldToScreenMatrix, const unsigned int color) {
//	Rect newRect = Transform2D::Homogeneous(rect, worldMatrix);
//	newRect = Transform2D::Homogeneous(newRect, worldToScreenMatrix);
//
//	Novice::DrawQuad(
//		int(newRect[3].x), int(newRect[3].y),
//		int(newRect[2].x), int(newRect[2].y),
//		int(newRect[1].x), int(newRect[1].y),
//		int(newRect[0].x), int(newRect[0].y),
//		int(spriteSize.x * frame), 0, int(spriteSize.x), int(spriteSize.y),
//		GameTextureManager::GetHandle(textureName), color
//	);
//}
//
//void Renderer::DrawBitmapAnimation(const Rect& rect, const Vector2& spriteSize, const std::string& textureName, const int bitmapNum, const int frame, const Matrix3x3& worldMatrix, const Matrix3x3& worldToScreenMatrix, const unsigned int color) {
//	Rect newRect = Transform2D::Homogeneous(rect, worldMatrix);
//	newRect = Transform2D::Homogeneous(newRect, worldToScreenMatrix);
//
//	Novice::DrawQuad(
//		int(newRect[3].x), int(newRect[3].y),
//		int(newRect[2].x), int(newRect[2].y),
//		int(newRect[1].x), int(newRect[1].y),
//		int(newRect[0].x), int(newRect[0].y),
//		int(spriteSize.x * frame), int(spriteSize.x * bitmapNum), int(spriteSize.x), int(spriteSize.y),
//		GameTextureManager::GetHandle(textureName), color
//	);
//}
//
//void Renderer::DrawUI(const Rect& rect, const Vector2& spriteSize, const std::string& textureName, const int frame) {
//	Rect newRect = Transform2D::Homogeneous(rect, Camera2D::GetPVMatrix());
//
//	Novice::DrawQuad(
//		int(newRect[3].x), int(newRect[3].y),
//		int(newRect[2].x), int(newRect[2].y),
//		int(newRect[1].x), int(newRect[1].y),
//		int(newRect[0].x), int(newRect[0].y),
//		int(spriteSize.x * frame), 0, int(spriteSize.x), int(spriteSize.y),
//		GameTextureManager::GetHandle(textureName), WHITE
//	);
//}

void Renderer::DrawLine(const Vector2& initial, const Vector2& terminal, unsigned int color) {
	Novice::DrawLine(int(initial.x), int(initial.y), int(terminal.x), int(terminal.y), color);
}

void Renderer::DrawLine(const Vector3& initial, const Vector3& terminal, unsigned int color) {
	Novice::DrawLine(int(initial.x), int(initial.y), int(terminal.x), int(terminal.y), color);
}

//void Renderer::DrawLineWidth(const Vector2& initial, const Vector2& terminal, float width, unsigned int color) {
//	if ((terminal - initial).length() != 0) {
//		Vector2 v = (terminal - initial).normalize();
//		Vector2 w_v1 = Vector2::Rotate(v, -1, 0) * (width / 2);
//		Novice::DrawQuad(
//			int(initial.x + w_v1.x), int(initial.y + w_v1.y),
//			int(terminal.x + w_v1.x), int(terminal.y + w_v1.y),
//			int(initial.x - w_v1.x), int(initial.y - w_v1.y),
//			int(terminal.x - w_v1.x), int(terminal.y - w_v1.y),
//			0, 0, int(initial.x - terminal.x), int(width),
//			GameTextureManager::GetHandle("white1x1"), color
//		);
//	}
//}