#pragma once

#include "Transform3D.h"
#include "Vector3D.h"
#include "Color.h"
#include <vector>

class Sphere {
public:
	Sphere() = default;
	Sphere(const Transform3D& transform, const Color& color_, float radius_, uint32_t split_);
	~Sphere() = default;

public:
	void begin();
	void update();
	void begin_rendering();
	void draw() const;

#ifdef _DEBUG
	void debug() const;
	void debug_gui();
#endif // _DEBUG

private:
	void create_vertexes();

public:
	void set_radius(float radius_);
	float get_radius() const;
	Transform3D& get_transform() { return transform; }
	const Transform3D& get_transform() const { return transform; }
	void set_color(const Color& color);

private:
	Transform3D transform;
	Color color;
	float radius;
	uint32_t split;
	std::vector<std::array<Vector3, 3>> vertexes;
	std::vector<std::array<Vector3, 3>> screenVertexes;
};

