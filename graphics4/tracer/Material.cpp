#include "Material.h"

Material::Material(): color(1, 1, 1)
{
}

Material::Material(const Color &color, double ambient, double diffuse, double reflection)
{
	this->color = color;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->reflection = reflection;
}
