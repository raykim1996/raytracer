#include <scene/materials/material.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <QString>
#include <QImage>

#include <tinyobj/tiny_obj_loader.h>

Material::Material() : 
	name("Material"), type(MaterialType::LAMBERT),
	baseColor(glm::vec3(0.1f, 0.1f, 0.1f)),
	emissive(false), reflectivity(0), iorIn(0), iorOut(0),
	texture(nullptr), normalMap(nullptr), ks(0), n(0)
{}
