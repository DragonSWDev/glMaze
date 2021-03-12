CXX ?= g++
TARGET = glmaze
CPPFILES = src/glad.cpp src/ShaderManager.cpp src/MazeGenerator.cpp src/glmaze.cpp
OBJS = $(CPPFILES:.cpp=.o)
CXXFLAGS = -Wall -pedantic -std=c++11 -s -O2 -I./include
LIBS = -lSDL2 -lSDL2_image -lGL -ldl

all: $(TARGET)

%.o: %.cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $<

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)
