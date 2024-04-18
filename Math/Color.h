#pragma once

class Color final {
public:
	Color();
	Color(unsigned int hex);
	Color(unsigned int _red, unsigned int _green, unsigned int _blue, unsigned int _alpha);

public:
	unsigned int red;
	unsigned int green;
	unsigned int blue;
	unsigned int alpha;

public:
	const unsigned int hex() const;

public:
	static unsigned int ToHex(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha);
	static unsigned int LerpC(unsigned int hex1, unsigned int hex2, float t);
	static Color LerpC(const Color& color1, const Color& color2, float t);
};