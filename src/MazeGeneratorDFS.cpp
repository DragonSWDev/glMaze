#include "MazeGeneratorDFS.hpp"

#include <algorithm>

MazeGeneratorDFS::MazeGeneratorDFS(unsigned int size, std::string seed)
: MazeGenerator(size, seed)
{
    mazeSize = size;

    mazeSize += 2;

    allocateArray();
}

void MazeGeneratorDFS::allocateArray()
{
    mazeArray = new bool*[mazeSize];
    
    for(unsigned int i = 0; i < mazeSize; ++i)
        mazeArray[i] = new bool[mazeSize];
}

void MazeGeneratorDFS::generateMaze()
{
    //Clear array
    for(unsigned int i = 0; i < mazeSize; i++)
    {
        for(unsigned int j = 0; j < mazeSize; j++)
        {
            mazeArray[i][j] = true;
        }
    }

    unsigned int x, y, direction;

    x = getRandomNumber(3, mazeSize - 3);
    y = getRandomNumber(3, mazeSize - 3);
    direction = getRandomNumber(1, 4);

    mazeArray[x][y] = false;

    switch (direction)
    {
        case 1: //Up
            addPath(x, y - 1);
            break;

        case 2: //Down
            addPath(x, y + 1);
            break;
        
        case 3: //Left
            addPath(x - 1, y);
            break;

        case 4: //Right
            addPath(x + 1, y);
            break;
    }

    setExit();
    setStartPosition();
}


void MazeGeneratorDFS::addPath(unsigned int x, unsigned int y)
{
    //Check if we are out of bonds
    if (x >= mazeSize - 1 || x < 1 || y < 1 || y >= mazeSize - 1)
    {
        return;
    }

    //Check if this cell was visited
    if (!mazeArray[x][y])
        return;

    //Count visited neighbours
    int count = 0;

    if (mazeArray[x - 1][y] == false)
        count++;

    if (mazeArray[x + 1][y] == false)
        count++;

    if (mazeArray[x][y - 1] == false)
        count++;

    if (mazeArray[x][y + 1] == false)
        count++;

    if (count > 1)
        return;

    mazeArray[x][y] = false;

    //Add directions to list
    directions.push_back(1);
    directions.push_back(2);
    directions.push_back(3);
    directions.push_back(4);

    std::shuffle(directions.begin(), directions.end(), randomEngine);

    for (unsigned int i = 0; i < directions.size(); i++)
    {
        switch (directions[i])
        {
            case 1: //Up
                addPath(x, y - 1);
                break;

            case 2: //Down
                addPath(x, y + 1);
                break;

            case 3: //Left
                addPath(x - 1, y);
                break;

            case 4: //Right
                addPath(x + 1, y);
                break;
        }
    }
}
