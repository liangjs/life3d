LIBS = `wx-config --libs core,base,gl` -lGL
OBJ = app.o main.o core.o
BIN = life3d
CXXFLAGS = `wx-config --cxxflags` -g -Wall -std=c++11

all: ${BIN}

life3d: ${OBJ}
	g++ ${OBJ} -o $@ $(LIBS)
app.o: app.cpp app.h
	g++ $< -c -o $@ ${CXXFLAGS}
main.o: main.cpp main.h
	g++ $< -c -o $@ ${CXXFLAGS}
core.o: core.cpp core.h
	g++ $< -c -o $@ ${CXXFLAGS}
	
clean:
	rm -f ${OBJ}
