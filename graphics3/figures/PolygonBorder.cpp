#include "PolygonBorder.h"
#include "GL/glut.h"
#include <cmath>

const double pi = 3.14159265359;

PolygonBorder::PolygonBorder()
{
}

void PolygonBorder::Draw(int angles, float R)
{
	float px1 = R, px2 = 1, py1 = 0, py2 = 0;
	for (int i=1; i<=angles; ++i)
	{
		float npx2 = cos(2 * pi / angles * i);
		float npx1 = R * npx2;
		float npy2 = sin(2 * pi / angles * i);
		float npy1 = R * npy2;
		glBegin(GL_QUAD_STRIP);
		glNormal3f(0, 0, -1);
		glVertex3f(px1, py1, -0.5);
		glVertex3f(npx1, npy1, -0.5);
		glVertex3f(px2, py2, -0.5);
		glVertex3f(npx2, npy2, -0.5);
		glNormal3f(npy2 - py2, px2 - npx2, 0);
		glVertex3f(px2, py2, 0.5);
		glVertex3f(npx2, npy2, 0.5);
		glNormal3f(0, 0, 1);
		glVertex3f(px1, py1, 0.5);
		glVertex3f(npx1, npy1, 0.5);
		glNormal3f(py1 - npy1, npx1 - px1, 0);
		glVertex3f(px1, py1, -0.5);
		glVertex3f(npx1, npy1, -0.5);
		glEnd();
		px1 = npx1;
		px2 = npx2;
		py1 = npy1;
		py2 = npy2;
	}
}
