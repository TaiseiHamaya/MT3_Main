#include "GameInput.h"

#include "Definition.h"

#include <Novice.h>
#include <algorithm>
#include <cassert>

std::array<char, 256> GameInput::keys;
std::array<char, 256> GameInput::preKeys;
std::array<DoubleFlag, 3> GameInput::mouseState;
float GameInput::stickDiedZone;

void GameInput::Initialize() {
	Novice::SetJoystickDeadZone(0, 0, 0);
	keys.fill({ 0 });
	preKeys.fill({ 0 });
	mouseState.fill(DoubleFlag{});
	stickDiedZone = 0.3f;
}

void GameInput::Finalize() {
}


void GameInput::Update(void) {
	preKeys = std::move(keys);
	Novice::GetHitKeyStateAll(&keys[0]);
	for (int i = 0; i < mouseState.size(); ++i) {
		mouseState[i].begin();
		mouseState[i].set_flag(Novice::IsPressMouse(i));
	}
}

char* GameInput::GetKeys(void) {
	return &keys[0];
}

char* GameInput::GetPreKeys(void) {
	return &preKeys[0];
}

bool GameInput::IsPressKey(int key) {
	return keys[key];
}

bool GameInput::IsTriggerKey(int key) {
	return keys[key] && !preKeys[key];
}

bool GameInput::IsReleaseKey(int key) {
	return !keys[key] && preKeys[key];
}

bool GameInput::IsTriggerMouse(MouseButton button) {
	return mouseState[button].on_enter();
}

bool GameInput::IsPressMouse(MouseButton button) {
	return mouseState[button];
}

bool GameInput::IsReleaseMouse(MouseButton button) {
	return mouseState[button].on_exit();
}

const DoubleFlag& GameInput::MouseState(MouseButton button) {
	return mouseState[button];
}

Vector2 GameInput::InputWASD(void) {
	Vector2 temp = Vec2::kZero;
	if (keys[DIK_W]) {
		temp.y += 1;
	}
	if (keys[DIK_S]) {
		temp.y -= 1;
	}

	if (keys[DIK_A]) {
		temp.x -= 1;
	}
	if (keys[DIK_D]) {
		temp.x += 1;
	}
	return temp.length() != 0 ? temp.normalize() : temp;
}

void GameInput::InputWASD(Vector2* const inputVelocity) {
	*inputVelocity = InputWASD();
}

float GameInput::InputAD(void) {
	float temp = 0.0f;
	if (keys[DIK_A]) {
		temp -= 1;
	}
	if (keys[DIK_D]) {
		temp += 1;
	}
	return temp;
}

void GameInput::InputAD(float* const inputVelocityX) {
	*inputVelocityX = InputAD();
}

Vector2 GameInput::InputArrow(void) {
	Vector2 temp{ 0, 0 };
	if (keys[DIK_UP]) {
		temp.y += 1;
	}
	if (keys[DIK_DOWN]) {
		temp.y -= 1;
	}

	if (keys[DIK_LEFT]) {
		temp.x -= 1;
	}
	if (keys[DIK_RIGHT]) {
		temp.x += 1;
	}
	return temp.length() != 0 ? temp.normalize() : temp;
}

void GameInput::InputArrow(Vector2* const inputVelocity) {
	*inputVelocity = InputArrow();
}

Vector2 GameInput::GetMousePos(void) {
	int x, y;
	Novice::GetMousePosition(&x, &y);
	return { static_cast<float>(x), static_cast<float>(y) };
}

Vector2 GameInput::InputLeftStick(void) {
	int x, y;
	Novice::GetAnalogInputLeft(0, &x, &y);
	Vector2 temp = Vector2{ (static_cast<float>(x) + 0.5f) / 32767.5f, (static_cast<float>(-y) - 0.5f) / 32767.5f };
	return temp.length() > stickDiedZone ? temp : Vector2{ 0, 0 };
}

void GameInput::InputLeftStick(Vector2* inputVelocity) {
	*inputVelocity = InputLeftStick();
}

void GameInput::SetStickDiedZone(float length) {
	stickDiedZone = length;
}