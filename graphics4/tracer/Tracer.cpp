#include "Tracer.h"
#include <vector>
#include <cmath>
using namespace std;

#define Lights ((vector<Light*>*)lights)
const double eps = 1e-5;
const double eps_c = 2e-2;
const double pi = 3.14159265359;
const int steps = 200;
const int reflect_steps = 5;
const int ambient_steps = 5;

Tracer::Tracer(const Tracer&)
{
}

Tracer::Tracer()
{
	image = 0;
	lights = (void*)(new vector<Light*>);
	scene = new Scene();
}

Tracer::~Tracer()
{
	for (unsigned int i=0; i < Lights->size(); ++i)
		delete (*Lights)[i];
	delete Lights;
	delete scene;
	if (image)
		delete image;
}

void Tracer::addFigure(Figure *f)
{
	scene->addFigure(f);
}

void Tracer::addLight(Light* l)
{
	Lights->push_back(l);
}

Image& Tracer::getImage()
{
	return *image;
}

double Tracer::RayLength(Ray &ray)
{
	double len = 0;
	for (int count = 0; count < steps; ++count)
	{
		double dist = scene->getDistance(ray.p);
		if (dist < eps) return len;
		ray.Move(dist);
		len += dist;
	}
	return len;
}

Color Tracer::TraceRay(Ray ray)
{
	Color result;
	double k = 1;
	for (int count = 0;; ++count)
	{
		double flen = RayLength(ray);
		Vector normal = scene->getNormal(ray.p);
		Material material = scene->getMaterial(ray.p);
		double ambient_k = 0, ambient_v = 2e-1;
		for (int i=0; i<ambient_steps; ++i, ambient_v += 2e-1)
			ambient_k += scene->getDistance(ray.p + normal * ambient_v) / ambient_v;
		result += material.color * k * (ambient_k / ambient_steps) * material.ambient;
		//result += material.color * k * material.ambient;
		for (unsigned int i=0; i < Lights->size(); ++i)
		{
			Light* light = (*Lights)[i];
			Vector way = ray.p - light->pos;
			Ray lray(light->pos, way);
			double len = RayLength(lray);
			if (fabs(len - way.Abs()) < eps_c)
			{
				len = -11;
				result += material.color * material.diffuse * k
					* fabs(lray.v.ScalarMul(normal)) * light->color;
			}
		}
		ray.v = ray.v.Rotate(normal, pi) * (-1);
		k *= material.reflection;
		if (k < 1.0 / 255) return result;
		if (count >= reflect_steps) return result;
		ray.Move(eps_c);
	}
}

void Tracer::Trace(const Point &pos, const Vector& direction, const Vector& up,
		int x, int y, double scale)
{
	if (image) delete image;
	image = new Image(x, y);
	// Vectors
	Vector vx = up.VectorMul(direction);
	vx.Normalize();
	vx = vx * (-scale);
	Vector vy = up;
	vy.Normalize();
	vy = vy * scale;
	Vector base = direction - vx * (x * 0.5) - vy * (y * 0.5);
	// Ray counting
	if (scene->getDistance(pos) < 0)
	{
		image->makeBlack();
		return;
	}
	for (int i=0; i<x; ++i)
	{
		Vector xbase = base;
		for (int j=0; j<y; ++j)
		{
			image->setPixel(i, j, TraceRay(Ray(pos, xbase)));
			xbase = xbase + vy;
		}
		base = base + vx;
	}
}

void Tracer::UnloadAll()
{
	for (unsigned int i=0; i < Lights->size(); ++i)
		delete (*Lights)[i];
	delete Lights;
	delete scene;
	if (image) delete image;
	image = 0;
	scene = new Scene();
	lights = (void*)(new vector<Light*>);
}
