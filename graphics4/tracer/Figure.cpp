#include "Figure.h"

const double eps = 1e-3;

Figure::~Figure()
{
}

Vector Figure::getNormal(const Point &p) const
{
	double x1 = getDistance(Point(p.x - eps, p.y, p.z));
	double x2 = getDistance(Point(p.x + eps, p.y, p.z));
	double y1 = getDistance(Point(p.x, p.y - eps, p.z));
	double y2 = getDistance(Point(p.x, p.y + eps, p.z));
	double z1 = getDistance(Point(p.x, p.y, p.z - eps));
	double z2 = getDistance(Point(p.x, p.y, p.z + eps));
	Vector r(x2-x1, y2-y1, z2-z1);
	r.Normalize();
	return r;
}
