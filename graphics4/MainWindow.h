#ifndef __GRAPHICS4_MAINWINDOW__
#define __GRAPHICS4_MAINWINDOW__

#include <QMainWindow>
#include <QLabel>
#include <QKeyEvent>

#include "tracer/Tracer.h"
#include "SceneLoader.h"

class MainWindow: public QMainWindow
{
	Q_OBJECT
public:
	MainWindow();
	~MainWindow();
private:
	Point pos;
	Vector up;
	Vector dir;
	QLabel* imageLabel;
	Tracer* tracer;
	SceneLoader* loader;

	void RefreshImage();
	void keyPressEvent(QKeyEvent*);
	void mouseReleaseEvent(QMouseEvent*);
};

#endif
