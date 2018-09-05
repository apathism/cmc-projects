#ifndef __TRACER_FIGURE__
#define __TRACER_FIGURE__

#include "Point.h"
#include "Material.h"

class Figure
{
public:
	virtual double getDistance(const Point&) const = 0;
	Vector getNormal(const Point&) const;
	virtual const Material getMaterial(const Point&) const = 0;
	virtual ~Figure();
};

#endif
