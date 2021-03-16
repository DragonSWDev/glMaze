#include "MazeGenerator.hpp"

#include <chrono>
#include <iostream>

MazeGenerator::MazeGenerator(unsigned int size, std::string seed)
{
    mazeSize = size;

    if (seed.empty()) //Use time as seed
    {
        unsigned timeSeed = std::chrono::system_clock::now().time_since_epoch().count();
        randomEngine.seed(timeSeed);
    }
    else //Use provided seed
    {
        std::seed_seq stringSeed(seed.begin(), seed.end());
        randomEngine.seed(stringSeed);
    }
}

MazeGenerator::~MazeGenerator()
{
    //Clean up array memory
    for(unsigned int i = 0; i < mazeSize; ++i) 
        delete [] mazeArray[i];

    delete [] mazeArray;
}

//Get random value in provided range
int MazeGenerator::getRandomNumber(int min, int max)
{
    return std::uniform_int_distribution<int>{min, max}(randomEngine);
}

//Set start position for maze
void MazeGenerator::setStartPosition()
{
    //Generate start position
    startX = getRandomNumber(1, mazeSize - 1);
    startY = getRandomNumber(1, mazeSize - 1);

    //Generate until start position will be empty field inside maze
    while (mazeArray[startY][startX])
    {
        startX = getRandomNumber(1, mazeSize - 1);
        startY = getRandomNumber(1, mazeSize - 1);
    }
}

//Set exit on some border
void MazeGenerator::setExit()
{
    //Set exit on random border wall (and check if it's accessible inside maze)
    //Dont stop until we get exit
    bool foundExit = false;

    while (!foundExit)
    {
        int exitIndex = getRandomNumber(1, mazeSize - 1);
        Direction exitWall = MazeGenerator::Direction(getRandomNumber(0, 3));

        switch (exitWall)
        {
            case Direction::TOP:
                if (mazeArray[1][exitIndex] == false)
                {
                    mazeArray[0][exitIndex] = false;
                    foundExit = true;
                }

                break;

            case Direction::BOTTOM:
                if (mazeArray[mazeSize - 2][exitIndex] == false)
                {
                    mazeArray[mazeSize - 1][exitIndex] = false;
                    foundExit = true;
                }

                break;

            case Direction::LEFT:
                if (mazeArray[exitIndex][1] == false)
                {
                    mazeArray[exitIndex][0] = false;
                    foundExit = true;
                }

                break;

            case Direction::RIGHT:
                if (mazeArray[exitIndex][mazeSize - 2] == false)
                {
                    mazeArray[exitIndex][mazeSize - 1] = false;
                    foundExit = true;
                }

                break;
        }
    }
}

bool** MazeGenerator::getMazeArray()
{
    return mazeArray;
}

unsigned int MazeGenerator::getMazeSize()
{
    return mazeSize;
}

unsigned int MazeGenerator::getStartX()
{
    return startX;
}

unsigned int MazeGenerator::getStartY()
{
    return startY;
}
