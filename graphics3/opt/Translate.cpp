#include "Translate.h"
#include <GL/glut.h>

Translate::Translate(const Translate&)
{
}

Translate::Translate(float x, float y, float z)
{
	glTranslatef(x, y, z);
}
