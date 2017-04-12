#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/constants.hpp>
#include <raytracer/RaytraceEngine.h>
#include <scene/camera.h>
#include <scene/scene.h>
#include <raytracer/ray.h>
#include <raytracer/intersection.h>
#include <QImage>
#include <QColor>
#include <QDebug>

void RaytraceEngine::render(const Camera &camera, const Scene &scene, QImage &image,
                            uint8_t maxDepth, uint8_t samples) {

	// For each pixel, cast *samples* rays using traceRay.
	// Average the color over all samples from traceRay and set
	// color of pixel (SSAA).
	for (int i = 0; i < camera.width; ++i) {
		for (int j = 0; j < camera.height; ++j) {
			Ray ray = camera.raycast(i, j);
            glm::vec3 color = RaytraceEngine::traceRay(ray, scene, 0, maxDepth);
			QRgb *line = (QRgb *) image.scanLine(j);
			line += i;
			*line = qRgb(color.x, color.y, color.z);
		}
	}
}

glm::vec3 RaytraceEngine::traceRay(const Ray &ray, const Scene &scene, 
								   uint8_t depth, uint8_t maxDepth) {

	// Recursively traces a ray up to maxDepth. If the ray hits a geometry,
	// we cast light feeler rays, add up contribution, and divide by number
	// lights. If the geometry hit is not reflective or transmissive, we
	// stop even if current recursion depth isn't max depth. Otherwise we
	// reflect/refract the ray and continue.

	if (depth > maxDepth) { return glm::vec3(0, 0, 0); }

	Intersection isect = Intersection::getIntersection(ray, scene);
	if (isect.objectHit == nullptr) { return glm::vec3(25, 25, 25); }

	if (isect.objectHit->material->emissive) {
		return isect.objectHit->material->baseColor * glm::vec3(255);
	}

	switch (isect.objectHit->material->type) {
	case MaterialType::LAMBERT: {
		glm::vec3 total = glm::vec3(0);
	    for (auto &p : scene.lights) {
	        Ray feeler = isect.raycast(p->transform.translation);
			if (Intersection::getIntersection(feeler, scene).objectHit == p) {
				total += isect.objectHit->material->baseColor *
	                    glm::max(0.f, glm::dot(glm::normalize(isect.normal),
	                    			  glm::normalize(feeler.direction))) *
						glm::vec3(255);
			}
		}

		glm::vec3 rColor = glm::vec3(0);
		if (isect.objectHit->material->reflectivity > 0) {
			glm::vec3 reflectDirection = glm::reflect(ray.direction, isect.normal);
			Ray reflect;
			reflect.origin = isect.isectPoint + glm::vec3(0.01) * isect.normal;
			reflect.direction = reflectDirection;
			rColor = traceRay(reflect, scene, depth + 1, maxDepth);
		}

		total /= glm::vec3(scene.lights.size());
		total = total * glm::vec3(1 - isect.objectHit->material->reflectivity) + 
				rColor * glm::vec3(isect.objectHit->material->reflectivity);
		total = glm::clamp(total, 0.f, 255.f);
		return total;
		break;
	}
	case MaterialType::PHONG: {
		glm::vec3 diffuse = glm::vec3(0);
		glm::vec3 specular = glm::vec3(0);
		for (auto &p : scene.lights) {
			Ray feeler = isect.raycast(p->transform.translation);
			if (Intersection::getIntersection(feeler, scene).objectHit == p) {
				diffuse += isect.objectHit->material->baseColor *
	                    glm::max(0.f, glm::dot(glm::normalize(isect.normal),
	                    			  glm::normalize(feeler.direction))) *
						glm::vec3(255);

				glm::vec3 r = glm::reflect(feeler.direction, isect.normal);
				specular += glm::vec3(glm::pow(glm::max(0.f,
					glm::dot(glm::normalize(r), glm::normalize(ray.direction))),
					isect.objectHit->material->n)) * glm::vec3(255);
			}
		}

		glm::vec3 reflective = glm::vec3(0);
		if (isect.objectHit->material->reflectivity > 0) {
			glm::vec3 reflectDirection = glm::reflect(ray.direction, isect.normal);
			Ray reflect;
			reflect.origin = isect.isectPoint + glm::vec3(0.01) * isect.normal;
			reflect.direction = reflectDirection;
			reflective = traceRay(reflect, scene, depth + 1, maxDepth);			
		}
		diffuse = diffuse * glm::vec3(1 - isect.objectHit->material->reflectivity) +
					reflective * glm::vec3(isect.objectHit->material->reflectivity);
		glm::vec3 total = diffuse * glm::vec3(1 - isect.objectHit->material->ks) + 
						  specular * glm::vec3(isect.objectHit->material->ks);
		total /= glm::vec3(scene.lights.size());
		return total;
		break;
	}
	default:
		break;
	}


	return glm::vec3(0);

	// Iterate through all lights, and call isect.raycast(light position) to
	// get a light feeler ray. Calculate the intersection of each of these.
	// If the object hit is the light itself (not obstructed), determine
	// the color of the current pixel by calling
	// isect.objectHit->material.getColor(). Store this. If the object
	// is reflective or refractive, recursively call traceRay with a new
	// transformed ray.
}

QImage RaytraceEngine::generateAOPass(const Camera &camera, const Scene &scene,
									  int samples, float spread, float distance) {

	// Cast rays through each pixel. If the ray hits a geometry,
	// cast *samples* rays from the intersection point in a
	// cone (defined by spread, 1 is a hemisphere) oriented with
	// the normal of the intersection point. Count the number of
	// these samples that hit another geometry within *distance*.
	// Set the intensity of the pixel to be:
	// 				1 - (num_hit_samples / samples)
	// If the initial ray doesn't hit a geometry, make the pixel
	// completely transparent.
	// This resulting AO image can be blended (multiply) with
	// the rendered image to generate the desired effect.

}
