#ifndef __TRACER_TRANSLATE__
#define __TRACER_TRANSLATE__

#include "Figure.h"

class Translate: public Figure
{
	Figure* figure;
	double x, y, z;
public:
	Translate(Figure*, double, double, double);
	~Translate();
	double getDistance(const Point&) const;
	const Material getMaterial(const Point&) const;
};

#endif
