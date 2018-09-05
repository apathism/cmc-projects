#include "Point.h"
#include <cmath>

Point::Point()
{
	x = y = z = 0;
}

Point::Point(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Point Point::operator+ (const Point &p) const
{
	Point np;
	np.x = x + p.x;
	np.y = y + p.y;
	np.z = z + p.z;
	return np;
}

Point Point::operator- (const Point &p) const
{
	Point np;
	np.x = x - p.x;
	np.y = y - p.y;
	np.z = z - p.z;
	return np;
}

Point Point::operator* (double s) const
{
	Point np;
	np.x = x * s;
	np.y = y * s;
	np.z = z * s;
	return np;
}

double Point::Abs() const
{
	return sqrt(x*x + y*y + z*z);
}

void Point::Normalize()
{
	double r = Abs();
	if (fabs(r) < 1e-10) return;
	x /= r;
	y /= r;
	z /= r;
}

Point Point::VectorMul(const Point &p) const
{
	Point np;
	np.x = y * p.z - z * p.y;
	np.y = z * p.x - x * p.z;
	np.z = x * p.y - y * p.x;
	return np;
}

Point Point::Rotate(const Vector &v, double angle) const
{
	double acos = cos(angle);
	double asin = -sin(angle);
	double bcos = 1 - acos;
	Point f;
	f.x =
		x * (acos + bcos * v.x * v.x) +
		y * (bcos * v.x * v.y - asin * v.z) +
		z * (bcos * v.x * v.z + asin * v.y);
	f.y =
		x * (bcos * v.y * v.x + asin * v.z) +
		y * (acos + bcos * v.y * v.y) +
		z * (bcos * v.y * v.z - asin * v.x);
	f.z =
		x * (bcos * v.z * v.x - asin * v.y) +
		y * (bcos * v.z * v.y + asin * v.x) +
		z * (acos + bcos * v.z * v.z);
	return f;
}

double Point::ScalarMul(const Point &p) const
{
	return x * p.x + y * p.y + z * p.z;
}
