#include "Sphere.h"

Sphere::Sphere(double r)
{
	this->r = r;
}

double Sphere::getDistance(const Point &p) const
{
	return p.Abs() - r;
}
