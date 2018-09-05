#include "Cube.h"
#include <cmath>

Cube::Cube(const Vector &size)
{
	this->size = size;
}

double Cube::getDistance(const Point &p) const
{
	double mx = fmax(fabs(p.x)-size.x, 0);
	double my = fmax(fabs(p.y)-size.y, 0);
	double mz = fmax(fabs(p.z)-size.z, 0);
	return sqrt(mx*mx + my*my + mz*mz);
}
