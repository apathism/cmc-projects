#include "Primitive.h"

Primitive* Primitive::setMaterial(const Material &material)
{
	__imaterial = material;
	return this;
}

Primitive::~Primitive()
{
}

const Material Primitive::getMaterial(const Point&) const
{
	return __imaterial;
}
