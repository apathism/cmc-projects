#include "Polygon.h"
#include "Cylinder.h"
#include "GL/glut.h"

const int slices = 30;

Cylinder::Cylinder()
{
}

void Cylinder::Draw()
{
	GLUquadric *qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluCylinder(qobj, 0.5, 0.5, 1.0, slices, 1);

	glPushMatrix();
	glScalef(0.5, 0.5, 1);
	glPushMatrix();
	glRotatef(180.0, 0, 1, 0);
	Polygon::Draw(slices);
	glPopMatrix();
	glTranslatef(0.0, 0.0, 1.0);
	Polygon::Draw(slices);
	glPopMatrix();

	gluDeleteQuadric(qobj);
}
