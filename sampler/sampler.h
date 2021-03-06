#ifndef __SAMPLER_H__
#define __SAMPLER_H__

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <sampler/pcg32.h>

#include <raytracer/intersection.h>

enum class Warp { Square, Disk, Sphere, Hemi, HemiCos };

class Sampler {
public:
	Sampler();
	void generateSamples(int samples, std::vector<glm::vec3> &vec, Warp method);
	void transformSamples(Intersection isect, std::vector<glm::vec3> &points);

private:
	static glm::vec3 warpDisk(const glm::vec2 &sample);
	pcg32 random;
};

#endif // __SAMPLER_H__
