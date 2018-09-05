#ifndef __TRACER_SCENE__
#define __TRACER_SCENE__

#include "Figure.h"

class Scene: public Figure
{
	void* data;
public:
	Scene();
	~Scene();

	void addFigure(Figure*);

	double getDistance(const Point&) const;
	const Material getMaterial(const Point&) const;
};

#endif

