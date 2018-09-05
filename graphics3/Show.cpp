#include <ctime>
#include <cmath>
#include <GL/glut.h>

#include "libs/All.h"
#include "figures/All.h"
#include "opt/All.h"

#include "Show.h"

#ifndef GLUT_WHEEL_DOWN
#define GLUT_WHEEL_DOWN 3
#endif

#ifndef GLUT_WHEEL_UP
#define GLUT_WHEEL_UP 4
#endif

const double pi = 3.14159265359;
int time_h, time_m, time_s;
double camdist;
int width, height;
bool antialias;

GLuint TexFaceID;

void InitStatic()
{
	glNewList(1, GL_COMPILE);
	{
		LocalColor(1.0, 0.84, 0.0, 1.0);
		{
			Positioning
				<< Rotate(180, 1, 0, 0)
				<< Translate(0, 0, -0.1);
			Polygon::Draw(6);
		}
		{
			Positioning << Scale(1.2, 1.2, 0.2);
			PolygonBorder::Draw(6, 0.83);
		}
	}

	{
		LocalColor(0.9, 0.9, 0.9, 1.0);
		Positioning
			<< Scale(0.05, 0.05, 0.04);
		Cylinder::Draw();
	}

	{
		LocalColor(1.0, 0.75, 0.1, 1);
		Positioning
			<< Rotate(-90, 1, 0, 0)
			<< Scale(0.1, 0.075, 0.1)
			<< Translate(0, 1, 0);
		Cylinder::Draw();
	}
	{
		LocalColor(1.0, 0.75, 0.1, 1);
		Positioning
			<< Rotate(-90, 1, 0, 0)
			<< Scale(0.15, 0.1, 0.15)
			<< Translate(0, 1.075, 0);
		Cylinder::Draw();
	}

	glEnable(GL_TEXTURE_2D);
	{
		LocalColor(1, 1, 1, 1);
		Polygon::DrawTexture(6, 1.2);
	}
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	{
		LocalColor(1.0, 1.0, 1.0, 0.3);
		Positioning << Translate(0, 0, 0.05);
		Polygon::Draw(6);
	}
	glDisable(GL_BLEND);

	glEndList();
}

void DisplayP(float x, float y, float z)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(camdist, (float)width / height, 1.5, 5.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(x, y, z, 0, 0, -1, 0, 1, 0);
	tbMatrix();

	Background::Draw(0.5, 0.5, 0.5);

	{
		LocalColor(1.0, 0.0, 0.0, 1.0);
		Positioning
			<< Scale(0.01, 0.9, 0.01)
			<< Translate(0, 0.3, 0.025)
			<< Rotate(-time_s * 6.0, 0.0, 0.0, 1.0);
		Cuboid::Draw();
	}

	{
		LocalColor(0.0, 0.0, 0.0, 1.0);
		{
			Positioning
				<< Scale(0.05, 0.8, 0.01)
				<< Translate(0, 0.3, 0.015)
				<< Rotate(-time_m * 6.0 - time_s / 10.0, 0.0, 0.0, 1.0);
			Cuboid::Draw();
		}

		{
			Positioning
				<< Scale(0.05, 0.6, 0.01)
				<< Translate(0, 0.2, 0.005)
				<< Rotate(-time_h * 30.0 - time_m / 2.0 - time_s / 120.0, 0.0, 0.0, 1.0);
			Cuboid::Draw();
		}
	}
	glCallList(1);
	glFlush();
}

void Display()
{
	if (!antialias)
	{
		DisplayP(0, 0, 4);
		glutSwapBuffers();
		return;
	}
	glClear(GL_ACCUM_BUFFER_BIT);
	for (int i=-1; i<=1; ++i)
		for (int j=-1; j<=1; ++j)
		{
			DisplayP(
				i * camdist * 0.1 / width,
				j * camdist * 0.1 / height, 4.0);
			glAccum(GL_ACCUM, 1.0 / 9);
		}
	glAccum(GL_RETURN, 1.f);
	glutSwapBuffers();
}

void Reshape(GLint w, GLint h)
{
	width = w; height = h;
	tbReshape(w, h);
	glutPostRedisplay();
}

void Motion(int x, int y)
{
	tbMotion(x, y);
}

void Mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
		tbStartMotion(x, y, button, glutGet(GLUT_ELAPSED_TIME));
	else if (state == GLUT_UP && button == GLUT_LEFT_BUTTON)
		tbStopMotion(button, glutGet(GLUT_ELAPSED_TIME));
	else if (state == GLUT_DOWN && button == GLUT_WHEEL_DOWN)
	{
		camdist -= 3.0;
		if (camdist < 20.0) camdist = 20.0;
		glutPostRedisplay();
	}
	else if (state == GLUT_DOWN && button == GLUT_WHEEL_UP)
	{
		camdist += 3.0;
		if (camdist > 100.0) camdist = 100.0;
		glutPostRedisplay();
	}
	else if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON)
	{
		static bool fullscreen = false;
		static int sx = 500, sy = 500;
		if (fullscreen)
			glutReshapeWindow(sx, sy);
		else
		{
			sx = width;
			sy = height;
			glutFullScreen();
		}
		fullscreen = !fullscreen;
	}
}

void SetTime()
{
	time_t rawtime;
	struct tm *timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	if (timeinfo == NULL) return;
	time_h = timeinfo->tm_hour;
	time_m = timeinfo->tm_min;
	time_s = timeinfo->tm_sec;
	glutPostRedisplay();
}

void Timer(int data)
{
	SetTime();
	glutPostRedisplay();
	glutTimerFunc(1000, Timer, 0);
}

void Keyboard(unsigned char key, int, int)
{
	if (key == '\033')
		exit(0);
	else if (key == 'a')
	{
		if (antialias)
			glReadBuffer(GL_FRONT);
		else
			glReadBuffer(GL_BACK);
		antialias = !antialias;
	}
	else if (key == 'f')
	{
		static bool fog = false;
		if (fog)
			glDisable(GL_FOG);
		else
			glEnable(GL_FOG);
		fog = !fog;
	}
}

void InitTextures()
{
	const char *facefile = "img/face.bmp";
	Image img;
	glGenTextures(1, &TexFaceID);
	if (!ImageLoader::Load(facefile, img))
		return;
	glBindTexture(GL_TEXTURE_2D, TexFaceID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, img.width, img.height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img.data);
}

void Init()
{
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE | GLUT_ACCUM);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Clock");

	antialias = true;
	camdist = 50.0;
	tbInit();

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	GLfloat lambient[] = {0.0, 0.0, 0.0, 1};
	GLfloat ldiffuse[] = {1.0, 1.0, 1.0, 1};
	GLfloat lspecular[] = {0, 0, 0, 1};
	GLfloat lposition[] = {-0.5, 0, 1, 0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, lambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ldiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lspecular);
	glLightfv(GL_LIGHT0, GL_POSITION, lposition);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);

	GLfloat fcolor[] = {0.5, 0.5, 0.5, 1};
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogf(GL_FOG_DENSITY, 0.3);
	glFogf(GL_FOG_START, 20.0);
	glFogf(GL_FOG_END, 100.0);
	glFogfv(GL_FOG_COLOR, fcolor);

	SetTime();
	InitStatic();
	InitTextures();

	glutTimerFunc(1000, Timer, 0);
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutKeyboardFunc(Keyboard);

	glReadBuffer(GL_BACK);
}

Show::Show(int &argc, char **&argv)
{
	glutInit(&argc, argv);
	Init();
	glutMainLoop();
}
