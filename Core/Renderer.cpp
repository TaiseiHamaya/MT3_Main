#include "Renderer.h"

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

void Renderer::DrawLine(const Vector2& initial, const Vector2& terminal, unsigned int color) {
	Novice::DrawLine(int(initial.x), int(initial.y), int(terminal.x), int(terminal.y), color);
}

void Renderer::DrawLine(const Vector3& initial, const Vector3& terminal, unsigned int color) {
	Novice::DrawLine(int(initial.x), int(initial.y), int(terminal.x), int(terminal.y), color);
}
