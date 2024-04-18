#pragma once

#include <numbers>

class DoubleFlag {
public:
	DoubleFlag() noexcept;
	~DoubleFlag() = default;

public:
	inline operator bool(void) const noexcept {
		return flag & 0b01;
	}

	inline bool operator!(void) const noexcept {
		return !flag;
	}

public:
	void begin() noexcept;
	void set_flag(bool set) noexcept;

public:
	bool on_enter() noexcept;
	bool on_exit() noexcept;

private:
	std::uint8_t flag;
};

