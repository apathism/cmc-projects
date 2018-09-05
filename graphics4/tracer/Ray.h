#ifndef __TRACER_RAY__
#define __TRACER_RAY__

#include "Point.h"

struct Ray
{
	Point p;
	Vector v;

	Ray(const Point&, const Vector&);
	void Move(double);
};

#endif
