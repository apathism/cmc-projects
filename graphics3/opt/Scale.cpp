#include "Scale.h"
#include <GL/glut.h>

Scale::Scale(const Scale&)
{
}

Scale::Scale(float x, float y, float z)
{
	glScalef(x, y, z);
}
