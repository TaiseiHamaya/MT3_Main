#pragma once

#include <cassert>
#include <numbers>

#include "Matrix.h"
#include "Vector2D.h"

constexpr float PI = float(std::numbers::pi);
constexpr float PI2 = float(std::numbers::pi * 2.0);

const Vector2 operator*(const Vector2& vec, const Matrix<2, 2>& mat);

/// <summary>
/// メモリのdeleteとnullptr化
/// <param name="mem">開放するメモリの変数</param>
/// </summary>
template<class type>
void MemClear(type*& mem);


///-------------------///
///   ここから定義     ///
///-------------------///

template<class type>
inline void MemClear(type*& mem) {
	delete mem;
	mem = nullptr;
}

/// <summary>
/// 長方形
/// </summary>
class Rect {
public:
	Rect() = default;
	Rect(const Vector2& size);
	Rect(const Vector2& topLeft_, const Vector2& topRight_, const Vector2& bottomLeft_, const Vector2& bottomRight_);
	inline const Vector2& operator[](std::size_t index) const {
		assert(index <= 3);
		return *((&topLeft) + index);
	}

public:
	Vector2 topLeft;
	Vector2 topRight;
	Vector2 bottomLeft;
	Vector2 bottomRight;
};

class Animation {
public:
	Animation();
	Animation(int distance_, int total_seq_, bool is_loop_);

public:
	int now(int time) const {
		return is_loop ? time / distance % total_seq : (std::min)(time / distance, total_seq);
	}

	bool is_end(int time) const {
		return time / distance >= total_seq;
	}

public:
	int distance;
	int total_seq;
	bool is_loop;
};