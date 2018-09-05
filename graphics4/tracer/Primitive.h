#ifndef __TRACER_PRIMITIVE__
#define __TRACER_PRIMITIVE__

#include "Figure.h"

class Primitive: public Figure
{
	Material __imaterial;
public:
	const Material getMaterial(const Point&) const;
	Primitive* setMaterial(const Material&);
	virtual ~Primitive();
};

#endif
