#ifndef __TRACER_CUBE__
#define __TRACER_CUBE__

#include "Primitive.h"

class Cube: public Primitive
{
	Vector size;
public:
	Cube(const Vector&);
	double getDistance(const Point&) const;
};

#endif
