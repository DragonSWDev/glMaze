#ifndef MAZEGENERATORDFS_HPP
#define MAZEGENERATORDFS_HPP

#include "MazeGenerator.hpp"

#include <vector>

//Maze generator that uses deep first search algorithm
class MazeGeneratorDFS : public MazeGenerator
{
    private:
        std::vector<int> directions;

        void allocateArray();
        void addPath(unsigned int x, unsigned int y);

    public:
        MazeGeneratorDFS(unsigned int size, std::string seed = "");
        void generateMaze();
};

#endif
