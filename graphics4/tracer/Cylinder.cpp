#include "Cylinder.h"
#include <cmath>

Cylinder::Cylinder(double r, double h)
{
	this->r = r;
	this->h = h;
}

double Cylinder::getDistance(const Point &p) const
{
	double c1 = fmax(sqrt(p.x*p.x + p.y*p.y) - r, 0);
	double c2 = fmax(fabs(p.z) - h, 0);
	return sqrt(c1*c1 + c2*c2);
}
