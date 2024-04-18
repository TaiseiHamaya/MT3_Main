#include "DeviceData.h"

std::unique_ptr<DeviceData> DeviceData::instance;

DeviceData::DeviceData(float windowWidth, float windowHight) {
	windowSize = { windowWidth, windowHight };
	screenSize = {};
	//_Update();
}

void DeviceData::Initialize(int windowWidth, int windowHight) {
	instance.reset(new DeviceData(float(windowWidth), float(windowHight)));
}

Vector2 DeviceData::GetWindowSize() {
	return instance->windowSize;
}

Vector2 DeviceData::GetScreenSize() {
	return instance->screenSize;
}

//void DeviceData::Update() {
//	instance->_Update();
//}

//float DeviceData::ScreenTopOnWorld() {
//	return instance->world_screen_top;
//}
//
//float DeviceData::ScreenBottomOnWorld() {
//	return instance->world_screen_bottom;
//}
//
//float DeviceData::ScreenLeftOnWorld() {
//	return instance->world_screen_left;
//}
//
//float DeviceData::ScreenRightOnWorld() {
//	return instance->world_screen_right;
//}
//
//void DeviceData::_Update() {
//	Vector2 bottomLeft = Transform2D::Homogeneous(Vector2{ -1.0f,-1.0f }, Camera2D::GetVPMatrix().inverse());
//	Vector2 topRight = Transform2D::Homogeneous(Vector2{ 1.0f,1.0f }, Camera2D::GetVPMatrix().inverse());
//
//	world_screen_right = topRight.x;
//	world_screen_top = topRight.y;
//
//	world_screen_left = bottomLeft.x;
//	world_screen_bottom = bottomLeft.y;
//}
