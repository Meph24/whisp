

EXEC=whisp


csrc=$(wildcard *.c)
cppsrc=$(wildcard *.cpp) $(wildcard noise/*.cpp) $(wildcard noise/module/*.cpp) $(wildcard noise/model/*.cpp)
obj=$(cppsrc:.cpp=.o) $(csrc:.c=.o)

CXX=g++-9
CXXFLAGS += -std=c++1z -g3 
CXXFLAGS += -Wall -pthread

LDFLAGS+= -lGL -lGLEW
LDFLAGS+= -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lsfml-audio

all: $(EXEC)
	mv $(EXEC) ..

$(EXEC): $(obj)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $(EXEC)


clean:
	$(RM) *.o *~ noise/*.o noise/module/*.o noise/model/*.o


.PHONY: all clean
