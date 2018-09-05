#include "Displacement.h"
#include <cmath>

Displacement::Displacement(Figure* figure)
{
	this->figure = figure;
}

Displacement::~Displacement()
{
	delete figure;
}

double Displacement::getDistance(const Point &p) const
{
	return figure->getDistance(Point(
		p.x * cos(p.y / 5) - p.z * sin(p.y / 5),
		p.y,
		p.x * sin(p.y / 5) + p.z * cos(p.y / 5)));
}

const Material Displacement::getMaterial(const Point &p) const
{
	return figure->getMaterial(Point(
		p.x * cos(p.y / 5) - p.z * sin(p.y / 5),
		p.y,
		p.x * sin(p.y / 5) + p.z * cos(p.y / 5)));
}
