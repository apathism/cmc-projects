#include "MainWindow.h"
#include <cmath>

const double pi = 3.14159265359;

MainWindow::MainWindow()
{
	imageLabel = new QLabel;
	imageLabel->setBackgroundRole(QPalette::Base);
	imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	imageLabel->setScaledContents(true);
	setCentralWidget(imageLabel);
	/************************************/
	pos = Point(0, 0, -4);
	dir = Vector(0, 0, 2);
	up = Vector(0, 1, 0);
	tracer = new Tracer();
	(loader = new SceneLoader(tracer))->LoadScene(0);
	/************************************/
	setWindowTitle(tr("Scene Viewer"));
	resize(500, 500);
	RefreshImage();
}

MainWindow::~MainWindow()
{
	delete tracer;
	delete imageLabel;
	delete loader;
}

void MainWindow::RefreshImage()
{
	tracer->Trace(pos, dir, up, this->size().width(), this->size().height(), 0.01);
	const Image &result = tracer->getImage();
	QImage img(result.Height(), result.Width(), QImage::Format_RGB32);
	for (int i=0; i<result.Height(); ++i)
		for (int j=0; j<result.Width(); ++j)
		{
			const Color &color = result.getPixel(i, j);
			img.setPixel(i, j, qRgb(
				qRound(color.R * 255),
				qRound(color.G * 255),
				qRound(color.B * 255)));
		}
	imageLabel->setPixmap(QPixmap::fromImage(img));
	//img.save("output.jpg");
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == 'A')
	{
		dir = dir.Rotate(up, -pi/6);
		RefreshImage();
	}
	else if (event->key() == 'D')
	{
		dir = dir.Rotate(up, pi/6);
		RefreshImage();
	}
	else if (event->key() == 'S')
	{
		Vector t = dir.VectorMul(up);
		t.Normalize();
		dir = dir.Rotate(t, -pi/6);
		up = up.Rotate(t, -pi/6);
		RefreshImage();
	}
	else if (event->key() == 'W')
	{
		Vector t = dir.VectorMul(up);
		t.Normalize();
		dir = dir.Rotate(t, pi/6);
		up = up.Rotate(t, pi/6);
		RefreshImage();
	}
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		Ray ray(pos, dir);
		ray.Move(1);
		pos = ray.p;
		RefreshImage();
	}
	else if (event->button() == Qt::RightButton)
	{
		Ray ray(pos, dir);
		ray.Move(-1);
		pos = ray.p;
		RefreshImage();
	}
}
