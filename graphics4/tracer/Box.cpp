#include "Box.h"
#include <cmath>

const double eps = 1e-3;

Box::Box(Image* texture, double size, int scale)
{
	this->size = size;
	this->texture = texture;
	this->scale = scale;
	temp = size * 2 / scale;
}

double Box::getDistance(const Point &p) const
{
	return size - fmax(fabs(p.x), fmax(fabs(p.y), fabs(p.z)));
}

const Color Box::Interpolation(double x, double y) const
{
	x -= (int)(x / temp) * temp;
	y -= (int)(y / temp) * temp;
	x *= texture->Width() / temp;
	y *= texture->Height() / temp;
	int x1 = x;
	int y1 = y;
	int x2 = x1 + 1;
	int y2 = y1 + 1;
	int fx2 = x2 == texture->Width() ? 0 : x2;
	int fy2 = y2 == texture->Height() ? 0 : y2;
	return
		texture->getPixel(x1, y1) * (x - x1) * (y - y1) +
		texture->getPixel(x1, fy2) * (x - x1) * (y2 - y) +
		texture->getPixel(fx2, y1) * (x2 - x) * (y - y1) +
		texture->getPixel(fx2, fy2) * (x2 - x) * (y2 - y);
}

const Material Box::getMaterial(const Point &p) const
{
	Material m;
	m.ambient = 0.2;
	m.diffuse = 0.8;
	m.reflection = 0;
	if (fabs(size - fabs(p.x)) < eps)
		m.color = Interpolation(p.y + size, p.z + size);
	else if (fabs(size - fabs(p.y)) < eps)
		m.color = Interpolation(p.x + size, p.z + size);
	else if (fabs(size - fabs(p.z)) < eps)
		m.color = Interpolation(p.x + size, p.y + size);
	return m;
}

Box::~Box()
{
	delete texture;
}
