#ifndef MAZEGENERATOR_HPP
#define MAZEGENERATOR_HPP

#include <vector>
#include <random>

//Base class for maze generators
//Defines some common fields, virtual methods and implements some common methods
class MazeGenerator
{
    public:
        enum Direction { TOP = 0, BOTTOM = 1, LEFT = 2, RIGHT = 3 };

        MazeGenerator(unsigned int size, std::string seed = "");
        virtual ~MazeGenerator();

        virtual void generateMaze() = 0;
        bool** getMazeArray();
        
        unsigned int getMazeSize();
        unsigned int getStartX();
        unsigned int getStartY();
        unsigned int getEndX();
        unsigned int getEndY();
        Direction getEndBorder();

    protected:
        unsigned int mazeSize, startX, startY, endX, endY;
        bool** mazeArray;
        bool arrayAllocated;
        std::mt19937 randomEngine;
        Direction endBorder;

        virtual void allocateArray() = 0;
        int getRandomNumber(int min, int max);
        void setStartPosition();
        void setExit();
};

#endif
