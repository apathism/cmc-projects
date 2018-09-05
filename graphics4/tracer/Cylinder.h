#ifndef __TRACER_CYLINDER__
#define __TRACER_CYLINDER__

#include "Primitive.h"

class Cylinder: public Primitive
{
	double r, h;
public:
	Cylinder(double, double);
	double getDistance(const Point&) const;
};

#endif
