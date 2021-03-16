#ifndef MAZEGENERATORRD_HPP
#define MAZEGENERATORRD_HPP

#include "MazeGenerator.hpp"

//Maze generator that uses recursive division algorithm
class MazeGeneratorRD : public MazeGenerator
{
    public:
        MazeGeneratorRD(unsigned int size, std::string seed = "");
        void generateMaze();

    private:
        enum Orientation { VERTICAL = 0, HORIZONTAL = 1 };

        void allocateArray();

        void divideChamber(int startFieldX, int startFieldY, int endFieldX, int endFieldY, Orientation direction);
        Orientation setOrientation(int startFieldX, int startFieldY, int endFieldX, int endFieldY);
};

#endif
