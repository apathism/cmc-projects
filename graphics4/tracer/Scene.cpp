#include "Scene.h"
#include <vector>
#include <cmath>
using namespace std;

#define Data ((vector<Figure*>*)data)
static Material __static_material_nothing;

Scene::Scene()
{
	data = (void*)(new vector<Figure*>());
}

Scene::~Scene()
{
	for (unsigned int i=0; i < Data->size(); ++i)
		delete (*Data)[i];
	delete Data;
}

void Scene::addFigure(Figure* f)
{
	Data->push_back(f);
}

double Scene::getDistance(const Point &p) const
{
	double mdist = INFINITY;
	for (unsigned int i=0; i < Data->size(); ++i)
		mdist = fmin(mdist, (*Data)[i]->getDistance(p));
	return mdist;
}

const Material Scene::getMaterial(const Point &p) const
{
	if (Data->size() == 0)
		return __static_material_nothing;
	unsigned int min = 0;
	double mdist = (*Data)[0]->getDistance(p);
	for (unsigned int i=1; i < Data->size(); ++i)
	{
		double cdist = (*Data)[i]->getDistance(p);
		if (cdist < mdist)
		{
			min = i;
			mdist = cdist;
		}
	}
	return (*Data)[min]->getMaterial(p);
}
