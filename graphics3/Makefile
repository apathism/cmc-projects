CXX = g++
CXXFLAGS = -Wall -O3
SOURCES = 	opt/Transformator.cpp \
		opt/Scale.cpp \
		opt/Translate.cpp \
		opt/Rotate.cpp \
		opt/Color.cpp \
		figures/Background.cpp \
		figures/Cuboid.cpp \
		figures/Cylinder.cpp \
		figures/Polygon.cpp \
		figures/PolygonBorder.cpp \
		libs/trackball.cpp \
		libs/ImageLoader.cpp \
		Show.cpp \
		Main.cpp
LIBS = -LGL -lGL -lGLU -lglut
OBJECTS = ${SOURCES:.cpp=.o}

clock: ${OBJECTS}
	${CXX} -o $@ ${CXXFLAGS} ${OBJECTS} ${LIBS}
%.o: %.cpp
	${CXX} ${CXXFLAGS} -c -o $@ $<
clean:
	rm -f clock ${OBJECTS}
