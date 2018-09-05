#ifndef __TRACER_LIGHT__
#define __TRACER_LIGHT__

#include "Color.h"
#include "Point.h"

struct Light
{
	Color color;
	Point pos;

	Light(const Color&, const Point&);
};

#endif
