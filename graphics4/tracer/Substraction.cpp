#include "Substraction.h"
#include <cmath>

Substraction::Substraction(Figure* f1, Figure* f2)
{
	this->f1 = f1;
	this->f2 = f2;
}

Substraction::~Substraction()
{
	delete f1;
	delete f2;
}

double Substraction::getDistance(const Point &p) const
{
	double d1 = f1->getDistance(p);
	double d2 = f2->getDistance(p);
	return fmax(d1, -d2);
}

const Material Substraction::getMaterial(const Point &p) const
{
	return f1->getMaterial(p);
}
