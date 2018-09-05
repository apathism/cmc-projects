#ifndef __TRACER_ROTATE__
#define __TRACER_ROTATE__

#include "Figure.h"

class Rotate: public Figure
{
	Figure* figure;
	Vector v;
	double angle;
public:
	Rotate(Figure*, const Vector&, double);
	~Rotate();
	double getDistance(const Point&) const;
	const Material getMaterial(const Point&) const;
};

#endif
