#include "MazeGenerator.hpp"

#include <iostream>
#include <algorithm>

MazeGenerator::MazeGenerator(unsigned int size)
{
    mazeSize = size;

    //Generate new array
    //We make it bigger to add 1 field of margin on all four sides
    //This will make rendering easier because there won't be need to carry about border as it will be drawn with rest of walls
    mazeArray = new bool*[mazeSize + 2];
    
    for(unsigned int i = 0; i < mazeSize + 2; ++i)
        mazeArray[i] = new bool[mazeSize + 2];
}

MazeGenerator::~MazeGenerator()
{
    //Clean up array memory
    for(unsigned int i = 0; i < mazeSize + 2; ++i) 
        delete [] mazeArray[i];

    delete [] mazeArray;
}

void MazeGenerator::generateMaze()
{
    std::cout << "Starting generation..." << std::endl;

    //Clear array
    for(unsigned int i = 0; i < mazeSize + 2; i++)
        for(unsigned int j = 0; j < mazeSize + 2; j++)
            mazeArray[i][j] = true;

    unsigned int x, y, direction, exit;

    x = randNum(3, mazeSize - 3);
    y = randNum(3, mazeSize - 3);
    direction = randNum(1, 4);

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

    //Set exit on random border wall (and check if it's accessible inside maze)
    //Dont stop until we get exit
    for (;;)
    {
        exit = randNum(2, mazeSize - 3);
        direction = randNum(1, 4);

        if (direction == 1) //Top border
            if (mazeArray[2][exit] == false)
            {
                mazeArray[1][exit] = false;
                break;
            }

        if (direction == 2) //Bottom border
            if (mazeArray[mazeSize - 3][exit] == false)
            {
                mazeArray[mazeSize - 2][exit] = false;
                break;
            }

        if (direction == 3) //Left border
            if (mazeArray[exit][2] == false)
            {
                mazeArray[exit][1] = false;
                break;
            }

        if (direction == 4) //Right border
            if (mazeArray[exit][mazeSize - 3] == false)
            {
                mazeArray[exit][mazeSize - 2] = false;
                break;
            }
    }

    //Generate start position
    startX = randNum(3, mazeSize - 3);
    startY = randNum(3, mazeSize - 3);

    //Generate until start position will be empty field inside maze
    while (mazeArray[startY][startX])
    {
        startX = randNum(3, mazeSize-3);
        startY = randNum(3, mazeSize-3);
    }

    std::cout << "Generation done." << std::endl;
}

bool** MazeGenerator::getMazeArray()
{
    return mazeArray;
}

void MazeGenerator::addPath(unsigned int x, unsigned int y)
{
    //Check if we are out of bonds
    if (x >= mazeSize - 2 || x <= 1 || y <= 1 || y >= mazeSize - 2)
        return;

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

    std::random_shuffle(directions.begin(), directions.end());

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

int MazeGenerator::randNum(int min, int max)
{
    return rand()%(max-min+1)+min;
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
