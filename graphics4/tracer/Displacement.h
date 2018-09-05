#ifndef __TRACER_DISPLACEMENT__
#define __TRACER_DISPLACEMENT__

#include "Figure.h"

class Displacement: public Figure
{
	Figure* figure;
public:
	Displacement(Figure*);
	~Displacement();
	double getDistance(const Point&) const;
	const Material getMaterial(const Point&) const;
};

#endif
