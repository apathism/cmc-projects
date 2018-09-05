#include "Translate.h"

Translate::Translate(Figure* figure, double x, double y, double z)
{
	this->figure = figure;
	this->x = x;
	this->y = y;
	this->z = z;
}

Translate::~Translate()
{
	delete figure;
}

double Translate::getDistance(const Point &p) const
{
	return figure->getDistance(p-Vector(x, y, z));
}

const Material Translate::getMaterial(const Point &p) const
{
	return figure->getMaterial(p-Vector(x, y, z));
}
