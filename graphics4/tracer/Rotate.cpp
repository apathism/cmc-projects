#include "Rotate.h"

Rotate::Rotate(Figure* figure, const Vector &v, double angle)
{
	this->figure = figure;
	this->v = v;
	this->v.Normalize();
	this->angle = angle;
}

Rotate::~Rotate()
{
	delete figure;
}

double Rotate::getDistance(const Point &p) const
{
	return figure->getDistance(p.Rotate(v, angle));
}

const Material Rotate::getMaterial(const Point &p) const
{
	return figure->getMaterial(p.Rotate(v, angle));
}
