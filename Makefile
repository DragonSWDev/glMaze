TARGET = glmaze
CPPFILES = src/glad.cpp src/ShaderManager.cpp src/MazeGenerator.cpp src/MazeGeneratorDFS.cpp src/MazeGeneratorRD.cpp src/glmaze.cpp
OBJS = $(CPPFILES:.cpp=.o)
CXXFLAGS = -Wall -pedantic -std=c++11 -O2 -I./include
LDFLAGS = -s -lSDL2 -lSDL2_image -lGL -ldl

all: $(TARGET)

%.o: %.cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $<

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LDFLAGS)

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)
