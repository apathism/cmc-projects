#include "Color.h"
#include <GL/glut.h>

Color::Color(float r, float g, float b, float a)
{
	glGetFloatv(GL_CURRENT_COLOR, color);
	glColor4f(r, g, b, a);
}

Color::~Color()
{
	glColor4fv(color);
}
