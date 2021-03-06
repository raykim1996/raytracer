#ifndef __BOUNDING_BOX_H__
#define __BOUNDING_BOX_H__

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <raytracer/ray.h>

class Geometry;

enum class Axis { X = 0, Y = 1, Z = 2 };

class BoundingBox {
public:
	BoundingBox() : minPoint(glm::vec3(0)), maxPoint(glm::vec3(0)) {}
	BoundingBox(const std::vector<Geometry *> &primitives);
	bool intersect(const Ray &ray, float &t) const;
	static BoundingBox Union(const BoundingBox &bb1, const BoundingBox &bb2);
	Axis getLongestAxis() const;
	glm::vec3 getMidpoint() const;
	float getVolume() const;

	glm::vec3 minPoint;
	glm::vec3 maxPoint;
};

#endif // __BOUNDING_BOX_H__