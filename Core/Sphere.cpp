#include "Sphere.h"

#include <Camera3D.h>
#include <Easing.h>
#include <Renderer.h>

#ifdef _DEBUG
#include "imgui.h"
#include <format>
#endif // _DEBUG

Sphere::Sphere(const Transform3D& transform_, const Color& color_, float radius_, uint32_t split_) {
	transform = transform_;
	color = color_;
	radius = radius_;
	split = split_;
	create_vertexes();
}

void Sphere::begin() {
	transform.begin();
}

void Sphere::update() {
}

void Sphere::begin_rendering() {
	transform.update();
	Matrix4x4 wvpv = transform.get_matrix() * Camera3D::GetVPOVMatrix();
	for (int i = 0; i < vertexes.size(); ++i) {
		screenVertexes[i][0] = Transform3D::Homogeneous(vertexes[i][0], wvpv);
		screenVertexes[i][1] = Transform3D::Homogeneous(vertexes[i][1], wvpv);
		screenVertexes[i][2] = Transform3D::Homogeneous(vertexes[i][2], wvpv);
	}
}

void Sphere::draw() const {
	unsigned int c = color.hex();
	for (int i = 0; i < vertexes.size(); ++i) {
		Renderer::DrawLine(screenVertexes[i][0], screenVertexes[i][1], c);
		Renderer::DrawLine(screenVertexes[i][0], screenVertexes[i][2], c);
	}
}

#ifdef _DEBUG
void Sphere::debug() const {
	transform.debug_axis(transform.get_matrix() * Camera3D::GetVPOVMatrix());
}

void Sphere::debug_gui() {
	ImGui::Text(std::format("Split : {:}", split).c_str());
	if (ImGui::DragFloat("Radius", &radius, 0.01f, 0.0f, 1000000.0f, "%.5f")) {
		create_vertexes();
	}
	transform.debug_gui();
	color.debug_gui();
}
#endif // _DEBUG

void Sphere::create_vertexes() {
	vertexes.clear();
	screenVertexes.clear();
	float latAngle = PI / split; // 緯度(-PI/2-PI/2)
	float lonAngle = PI2 / split; // 経度(0-PI2)
	for (uint32_t latIndex = 0; latIndex < split; ++latIndex) {
		float lat = Lerp(-PI / 2, PI / 2, float(latIndex) / split);
		for (uint32_t lonIndex = 0; lonIndex < split; ++lonIndex) {
			float lon = Lerp(0.0f, PI2, float(lonIndex) / split);
			vertexes.push_back({});
			vertexes.back()[0] = Vector3{ std::cos(lat) * std::cos(lon), std::sin(lat), std::cos(lat) * std::sin(lon) } *radius;
			vertexes.back()[1] = Vector3{ std::cos(lat + latAngle) * std::cos(lon), std::sin(lat + latAngle), std::cos(lat + latAngle) * std::sin(lon) }*radius;
			vertexes.back()[2] = Vector3{ std::cos(lat) * std::cos(lon + lonAngle), std::sin(lat), std::cos(lat) * std::sin(lon + lonAngle) }*radius;
		}
	}
	screenVertexes.resize(vertexes.size());
}

void Sphere::set_radius(float radius_) {
	radius = radius_;
	create_vertexes();
}

float Sphere::get_radius() const {
	return radius;
}

void Sphere::set_color(const Color& color_) {
	color = color_;
}
