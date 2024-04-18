#include "Color.h"

#include "Easing.h"

#include <algorithm>

Color::Color() {
	red = 0;
	green = 0;
	blue = 0;
	alpha = 0;
}

Color::Color(unsigned int hex) {
	red = (hex >> 24) & 0x000000ff;
	green = (hex >> 16) & 0x000000ff;
	blue = (hex >> 8) & 0x000000ff;
	alpha = (hex >> 0) & 0x000000ff;
}

Color::Color(unsigned int _red, unsigned int _green, unsigned int _blue, unsigned int _alpha) {
	red = _red;
	green = _green;
	blue = _blue;
	alpha = _alpha;
}

const unsigned int Color::hex() const {
	return (red << 24) + (green << 16) + (blue << 8) + alpha;
}

unsigned int Color::ToHex(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha) {
	return (red << 24) + (green << 16) + (blue << 8) + alpha;
}

unsigned int Color::LerpC(unsigned int hex1, unsigned int hex2, float t) {
	return Color::ToHex(
		std::clamp(Lerp((hex1 >> 24) & 0x000000ff, (hex2 >> 24) & 0x000000ff, t), 0x0u, 0xffffffff),
		std::clamp(Lerp((hex1 >> 16) & 0x000000ff, (hex2 >> 16) & 0x000000ff, t), 0x0u, 0xffffffff),
		std::clamp(Lerp((hex1 >> 8) & 0x000000ff, (hex2 >> 8) & 0x000000ff, t), 0x0u, 0xffffffff),
		std::clamp(Lerp(((hex1 >> 0) & 0x000000ff), (hex2 >> 0) & 0x000000ff, t), 0x0u, 0xffffffff));
}

Color Color::LerpC(const Color& color1, const Color& color2, float t) {
	return Color{
		Lerp(color1.red, color2.red, t),
		Lerp(color1.green, color2.green, t),
		Lerp(color1.blue, color2.blue, t),
		Lerp(color1.alpha, color2.alpha, t)
	};
}
