#include "Polygon.h"
#include "GL/glut.h"
#include <cmath>

const double pi = 3.14159265359;

Polygon::Polygon()
{
}

void Polygon::Draw(int angles)
{
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, 1);
	for (int i=0; i<angles; ++i)
		glVertex3f(
			cos(2 * pi * i / angles),
			sin(2 * pi * i/ angles), 0);
	glEnd();
}

void Polygon::DrawTexture(int angles, float texsize)
{
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, 1);
	for (int i=0; i<angles; ++i)
	{
		float x = cos(2 * pi * i / angles);
		float y = sin(2 * pi * i / angles);
		glTexCoord2f(
			(x * texsize + 1) / 2,
			(y * texsize + 1) / 2);
		glVertex3f(x, y, 0);
	}
	glEnd();
}
