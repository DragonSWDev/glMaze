#include "MazeGeneratorRD.hpp"

#include <iostream>

MazeGeneratorRD::MazeGeneratorRD(unsigned int size, std::string seed)
: MazeGenerator(size, seed)
{
    mazeSize = size;

    if (mazeSize % 2 == 0) //Array size must be odd to generate proper maze with this implementation
    {
        mazeSize++;
    }

    allocateArray();
}

void MazeGeneratorRD::allocateArray()
{
    mazeArray = new bool*[mazeSize];
    
    for(unsigned int i = 0; i < mazeSize; ++i)
        mazeArray[i] = new bool[mazeSize];
}

void MazeGeneratorRD::generateMaze()
{
    //Init array
    for (unsigned int i = 0; i < mazeSize; i++)
    {
        for (unsigned int j = 0; j < mazeSize; j++)
        {
            mazeArray[i][j] = false;
        }
    }

    //Make border
    for (unsigned int i = 0; i < mazeSize; i++)
    {
        mazeArray[i][0] = true; //Left
        mazeArray[0][i] = true; //Top
        mazeArray[i][mazeSize - 1] = true; //Right
        mazeArray[mazeSize - 1][i] = true; //Bottom
    }

    //Get count of maze fields in allocated array
    //Maze fields are array fields with odd index
    int mazeFields = (mazeSize - 1) / 2;

    divideChamber(0, 0, mazeFields - 1, mazeFields - 1, Orientation(getRandomNumber(0, 1)));

    setExit();
    setStartPosition();
}

//Divide chamber into two chambers
//After dividing run this function again for each chamber
void MazeGeneratorRD::divideChamber(int startFieldX, int startFieldY, int endFieldX, int endFieldY, Orientation direction)
{
    if ((endFieldX - startFieldX) < 1 || (endFieldY - startFieldY) < 1)
    {
        return;
    }

    //Draw horizontal wall
    if (direction == Orientation::HORIZONTAL)
    {
        int wallField = getRandomNumber(startFieldY, endFieldY - 1);

        //Get array index of randomly selected maze field
        int wallIndex = wallField * 2 + 1;
        wallIndex++; //Wall will be drawn in position next to the selected field

        for (int i = startFieldX * 2 + 1; i < endFieldX * 2 + 2; i++) //Draw horizontal wall
        {
            mazeArray[wallIndex][i] = true;
        }

        int passageField = getRandomNumber(startFieldX, endFieldX); //Select maze field where passage will be placed

        mazeArray[wallIndex][passageField * 2 + 1] = false; //Put passage on wall

        //There are two chambers divided by horizontal wall
        
        //Divide created chambers
        divideChamber(startFieldX, startFieldY, endFieldX, wallField, setOrientation(startFieldX, startFieldY, endFieldX, wallField));
        divideChamber(startFieldX, wallField + 1, endFieldX, endFieldY, setOrientation(startFieldX, wallField + 1, endFieldX, endFieldY));
    }
    else //Drawing vertical wall
    {
        int wallField = getRandomNumber(startFieldX, endFieldX - 1);

        //Same as before but vertically
        int wallIndex = wallField * 2 + 1;
        wallIndex++;

        for (int i = startFieldY * 2 + 1; i < endFieldY * 2 + 2; i++)
        {
            mazeArray[i][wallIndex] = true;
        }

        int passageField = getRandomNumber(startFieldY, endFieldY);

        mazeArray[passageField * 2 + 1][wallIndex] = false;

        
        divideChamber(startFieldX, startFieldY, wallField, endFieldY, setOrientation(startFieldX, startFieldY, wallField, endFieldY));
        divideChamber(wallField + 1, startFieldY, endFieldX, endFieldY, setOrientation(startFieldX, startFieldY, wallField, endFieldY));
    }
}

//Choose wall orientation
//If chamber is wider than higher divide it vertically
//If chamber is higher than wider divide it horizontally
//If chamber width and height are same then set random orientation
MazeGeneratorRD::Orientation MazeGeneratorRD::setOrientation(int startFieldX, int startFieldY, int endFieldX, int endFieldY)
{
    int chamberWidth = endFieldX - startFieldX;
    int chamberHeight = endFieldY - startFieldY;

    if (chamberWidth > chamberHeight)
    {
        return Orientation::VERTICAL;
    } 
    
    if (chamberWidth < chamberHeight)
    {
        return Orientation::HORIZONTAL;
    } 

    return Orientation(getRandomNumber(0, 1));
}
