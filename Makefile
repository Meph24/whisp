
csrc=$(wildcard *.c)
cppsrc=$(wildcard *.cpp)
obj=$(cppsrc:.cpp=.o) $(csrc:.c=.o)

CXX=g++
CXXFLAGS=-std=c++14 -Wall -pthread
EXEC=whisp

LDFLAGS+=-lGL -lGLEW
LDFLAGS+=-lsfml-graphics -lsfml-window -lsfml-system

all: $(obj)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $(EXEC)

.PHONY: clean
clean:
	rm -f $(obj)

