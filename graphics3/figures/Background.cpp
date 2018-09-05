#include "Background.h"
#include <GL/glut.h>

Background::Background()
{
}

void Background::Draw(float r, float g, float b)
{
	glClearColor(r, g, b, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
