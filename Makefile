
EXEC=whisp


csrc=$(wildcard *.c)
cppsrc=$(wildcard *.cpp)
obj=$(cppsrc:.cpp=.o) $(csrc:.c=.o)

CXX=g++
CXXFLAGS += -std=c++14 
CXXFLAGS += -Wall -pthread

LDFLAGS+= -lGL -lGLEW
LDFLAGS+= -lsfml-graphics -lsfml-window -lsfml-system

all: $(EXEC)

$(EXEC): $(obj)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $(EXEC)


clean:
	$(RM) *.o *~


.PHONY: all clean
