#include "Ray.h"

Ray::Ray(const Point &p, const Vector &v)
{
	this->p = p;
	this->v = v;
	this->v.Normalize();
}

void Ray::Move(double dist)
{
	p = p + v * dist;
}
