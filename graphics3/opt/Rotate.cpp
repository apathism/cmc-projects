#include "Rotate.h"
#include <GL/glut.h>

Rotate::Rotate(const Rotate&)
{
}

Rotate::Rotate(float angle, float x, float y, float z)
{
	glRotatef(angle, x, y, z);
}
