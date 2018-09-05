#ifndef __TRACER_MATERIAL__
#define __TRACER_MATERIAL__

#include "Color.h"

struct Material
{
	Color color;
	double ambient;
	double diffuse;
	double reflection;

	Material();
	Material(const Color&, double, double, double);
};

#endif
