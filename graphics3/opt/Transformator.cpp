#include "Transformator.h"
#include <GL/glut.h>

Transformator::Transformator(const Transformator&)
{
}

Transformator::Transformator()
{
	glPushMatrix();
}

Transformator::~Transformator()
{
	glPopMatrix();
}

const Transformator& Transformator::operator<< (const Transformation &tr) const
{
	return *this;
}
