TEMPLATE=app
CONFIG += qt debug
SOURCES += \
	Main.cpp \
	SceneLoader.cpp \
	MainWindow.cpp \
	tracer/Point.cpp \
	tracer/Figure.cpp \
	tracer/Primitive.cpp \
	tracer/Sphere.cpp \
	tracer/Cube.cpp \
	tracer/Box.cpp \
	tracer/Cylinder.cpp \
	tracer/Translate.cpp \
	tracer/Rotate.cpp \
	tracer/Displacement.cpp \
	tracer/Substraction.cpp \
	tracer/Color.cpp \
	tracer/Material.cpp \
	tracer/Ray.cpp \
	tracer/Light.cpp \
	tracer/Image.cpp \
	tracer/Scene.cpp \
	tracer/Tracer.cpp

HEADERS += \
	SceneLoader.h \
	MainWindow.h \
	tracer/Point.h \
	tracer/Figure.h \
	tracer/Primitive.h \
	tracer/Sphere.h \
	tracer/Cube.h \
	tracer/Box.h \
	tracer/Cylinder.h \
	tracer/Translate.h \
	tracer/Rotate.h \
	tracer/Displacement.h \
	tracer/Substraction.h \
	tracer/Color.h \
	tracer/Material.h \
	tracer/Ray.h \
	tracer/Light.h \
	tracer/Image.h \
	tracer/Scene.h \
	tracer/Tracer.h

OBJECTS_DIR = temp/obj
MOC_DIR = temp/moc
