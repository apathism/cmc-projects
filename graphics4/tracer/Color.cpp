#include "Color.h"
#include <cmath>

Color::Color()
{
	R = G = B = 0.0;
}

Color::Color(double R, double G, double B)
{
	this->R = R;
	this->G = G;
	this->B = B;
}

Color Color::operator+ (const Color &c) const
{
	Color nc;
	nc.R = fmax(fmin(R + c.R, 1), 0);
	nc.G = fmax(fmin(G + c.G, 1), 0);
	nc.B = fmax(fmin(B + c.B, 1), 0);
	return nc;
}

Color Color::operator* (const Color &c) const
{
	Color nc;
	nc.R = R * c.R;
	nc.G = G * c.G;
	nc.B = B * c.B;
	return nc;
}

Color& Color::operator+= (const Color &c)
{
	R += c.R;
	G += c.G;
	B += c.B;
	R = fmax(fmin(R, 1), 0);
	G = fmax(fmin(G, 1), 0);
	B = fmax(fmin(B, 1), 0);
	return *this;
}

Color& Color::operator*= (const Color &c)
{
	R *= c.R;
	G *= c.G;
	B *= c.B;
	return *this;
}

Color Color::operator* (double x) const
{
	Color color;
	color.R = R*x;
	color.G = G*x;
	color.B = B*x;
	return color;
}
