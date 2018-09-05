#ifndef __TRACER_SUBSTRACTION__
#define __TRACER_SUBSTRACTION__

#include "Figure.h"

class Substraction: public Figure
{
	Figure* f1;
	Figure* f2;
public:
	Substraction(Figure*, Figure*);
	~Substraction();
	double getDistance(const Point&) const;
	const Material getMaterial(const Point&) const;
};

#endif
