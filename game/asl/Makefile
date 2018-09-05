CXX = g++
CXXFLAGS = -Wall -O3
SOURCES =	string.cpp		\
		messageexception.cpp	\
		syscallexception.cpp	\
		streamsocket.cpp	\
		pointerarray.cpp	\
		cloneable.cpp

OBJECTS = ${SOURCES:.cpp=.o}

libasl.a: ${OBJECTS}
	${AR} crs $@ ${OBJECTS}
%.o: %.cpp
	${CXX} ${CXXFLAGS} -c -o $@ $<
clean:
	rm -f libasl.a ${OBJECTS}
