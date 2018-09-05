#ifndef __TRACER_SPHERE__
#define __TRACER_SPHERE__

#include "Primitive.h"

class Sphere: public Primitive
{
	double r;
public:
	Sphere(double);
	double getDistance(const Point&) const;
};

#endif
