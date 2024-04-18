#include "DoubleFlag.h"

#include <numbers>

DoubleFlag::DoubleFlag() noexcept :
	flag(0) {
}

void DoubleFlag::begin() noexcept {
	flag <<= 1;
}

void DoubleFlag::set_flag(bool set) noexcept {
	flag |= static_cast<std::uint8_t>(set);
}

bool DoubleFlag::on_enter() noexcept {
	return flag == 0b01;
}

bool DoubleFlag::on_exit() noexcept {
	return flag == 0b10;
}