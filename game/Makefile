CXX = g++
CXXFLAGS = -Wall -O3
SOURCES = 	main.cpp		\
		connectionsettings.cpp	\
		gameplayer.cpp		\
		gamemarket.cpp		\
		gameclient.cpp		\
		gamecallback.cpp
OBJECTS = ${SOURCES:.cpp=.o}

bot: ${OBJECTS} asl/libasl.a script/libscript.a
	${CXX} -o $@ ${CXXFLAGS} ${OBJECTS} -Lasl -Lscript -lscript -lasl
asl/libasl.a:
	cd asl && ${MAKE}
script/libscript.a:
	cd script && ${MAKE}
%.o: %.cpp
	${CXX} ${CXXFLAGS} -c -o $@ $<
clean:
	rm -f bot ${OBJECTS}
	cd asl && ${MAKE} clean
	cd script && ${MAKE} clean
