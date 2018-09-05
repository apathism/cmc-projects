#ifndef __TRACER_BOX__
#define __TRACER_BOX__

#include "Figure.h"
#include "Image.h"

class Box: public Figure
{
	double size;
	int scale;
	double temp;
	Image* texture;
	const Color Interpolation(double, double) const;
public:
	Box(Image*, double, int);
	~Box();
	double getDistance(const Point&) const;
	const Material getMaterial(const Point&) const;
};

#endif
