#pragma once

#include "Transform3D.h"
#include "Vector3D.h"
#include <vector>

class Sphere {
public:
	Sphere() = default;
	Sphere(const Transform3D& transform, float radius_, uint32_t split_);
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
	Transform3D transform;
	float radius;
	uint32_t split;
	std::vector<std::array<Vector3, 3>> vertexes;
	std::vector<std::array<Vector3, 3>> screenVertexes;
};

