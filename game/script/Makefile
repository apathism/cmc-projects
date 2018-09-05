CXX = g++
CXXFLAGS = -Wall -O3
SOURCES =	lexem.cpp			\
		lexemanalyzer.cpp		\
		lexemanalyzerexception.cpp	\
		syntaxanalyzer.cpp		\
		syntaxanalyzerexception.cpp	\
		stackitem.cpp			\
		polizitem.cpp			\
		polizconst.cpp			\
		poliznop.cpp			\
		polizvar.cpp			\
		polizjump.cpp			\
		polizifjump.cpp			\
		polizclean.cpp			\
		polizmarker.cpp			\
		polizoperation.cpp		\
		polizfunction.cpp		\
		runtimeexception.cpp		\
		callback.cpp			\
		stdcallback.cpp			\
		script.cpp
		
OBJECTS = ${SOURCES:.cpp=.o}

interpreter: libscript.a asl/libasl.a interpreter.o
	${CXX} ${CXXFLAGS} -o $@ interpreter.o -L. -Lasl -lscript -lasl
libscript.a: ${OBJECTS}
	${AR} crs $@ ${OBJECTS}
asl/libasl.a:
	cd asl && ${MAKE}
%.o: %.cpp
	${CXX} ${CXXFLAGS} -c -o $@ $<
clean:
	rm -f interpreter libscript.a ${OBJECTS}
	cd asl && ${MAKE} clean
