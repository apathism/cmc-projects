#include "SceneLoader.h"

#include "tracer/Sphere.h"
#include "tracer/Cube.h"
#include "tracer/Translate.h"
#include "tracer/Rotate.h"
#include "tracer/Displacement.h"
#include "tracer/Substraction.h"
#include "tracer/Cylinder.h"
#include "tracer/Box.h"
#include "tracer/Image.h"
#include <cmath>
#include <QImage>

const double pi = 3.14159265359;

SceneLoader::SceneLoader(Tracer *tracer)
{
	this->tracer = tracer;
}

Image* __loadImage(const char *filename)
{
	QImage img(filename);
	Image *res = new Image(img.width(), img.height());
	for (int i=0; i<img.width(); ++i)
		for (int j=0; j<img.height(); ++j)
		{
			QRgb p = img.pixel(i, j);
			res->setPixel(i, j, Color(qRed(p) / 255.0, qGreen(p) / 255.0, qBlue(p) / 255.0));
		}
	return res;
}

void SceneLoader::LoadScene(int) const
{
	// Argument is unsupported. Sorry :(
	tracer->addFigure(new Translate((new Box(__loadImage("marble.jpg"), 10, 3)),
		0, -7, 0));
	tracer->addFigure(new Translate((new Sphere(1))->setMaterial(Material(Color(0, 0, 1), 0.1, 0.5, 0.4)),
		0, 1, 0
	));
	tracer->addFigure(
		new Substraction(
			new Translate(new Rotate(
				(new Cylinder(1, 2))->setMaterial(Material(Color(0, 1, 0), 0.2, 0.7, 0.1)),
				Vector(1, 0, 0), pi/2), 2, 1, 0
			),
			new Translate(
				(new Sphere(0.5))->setMaterial(Material(Color(0, 0, 1), 0.1, 0.5, 0.4)),
				2, 1, 1
			)
	));
	tracer->addFigure(new Translate(new Displacement(
		(new Cube(Point(1, 0.5, 1)))->setMaterial(Material(Color(1, 0, 0), 0.2, 0.8, 0.0))),
		0, 2.5, 0
	));
	tracer->addLight(new Light(Color(0.9, 1, 0.9), Point(-5, -5, 5)));
	tracer->addLight(new Light(Color(0.2, 0.2, 0.4), Point(2, -2, 2)));
}
