#ifndef __TRACER_TRACER__
#define __TRACER_TRACER__

#include "Image.h"
#include "Scene.h"
#include "Light.h"
#include "Ray.h"

class Tracer
{
	Scene* scene;
	void* lights;
	Image* image;

	Tracer(const Tracer&);

	double RayLength(Ray&);
	Color TraceRay(Ray);
public:
	Tracer();
	void addFigure(Figure*);
	void addLight(Light*);
	void Trace(const Point&, const Vector&, const Vector&,
		int, int, double);
	Image& getImage();
	void UnloadAll();
	~Tracer();
};

#endif
