#include "Cuboid.h"
#include "GL/glut.h"

Cuboid::Cuboid()
{
}

void Cuboid::Draw()
{
	glutSolidCube(1.0);
}
