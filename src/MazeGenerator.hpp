#ifndef MAZEGENERATOR_HPP
#define MAZEGENERATOR_HPP

#include <vector>

class MazeGenerator
{
    private:
        unsigned int mazeSize, startX, startY;
        bool** mazeArray;
        std::vector<int> directions;

        int randNum(int min, int max);
        void addPath(unsigned int x, unsigned int y);

    public:
        MazeGenerator(unsigned int size);
        ~MazeGenerator();
        void generateMaze();
        bool** getMazeArray();
        
        unsigned int getMazeSize();
        unsigned int getStartX();
        unsigned int getStartY();
};

#endif
