LIBS = `wx-config --libs core,base,gl`
OBJ = app.o main.o
BIN = life3d
CXXFLAGS = `wx-config --cxxflags` -g -Wall

all: ${BIN}

life3d: ${OBJ}
	g++ ${OBJ} -o $@ $(LIBS)
app.o: app.cpp app.h
	g++ $< -c -o $@ ${CXXFLAGS}
main.o: main.cpp main.h
	g++ $< -c -o $@ ${CXXFLAGS}

clean:
	rm -f ${OBJ}
