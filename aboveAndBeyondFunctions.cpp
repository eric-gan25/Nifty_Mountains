#include <iostream>
#include <apmatrix.h>
#include <fstream>
#include <math.h>
#include <time.h>
#include "header.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>

using namespace std;

// Takes in mountain matrix data, starting row, and colour as inputs
// From a starting row it draws the path through using the lowest elevation algorithm
// Returns total elevation change of the path
int lowestElevation(apmatrix<short> &gridMap, int row, ALLEGRO_COLOR colour)
{
    srand(time(0));

    int totalElevation = 0;
    int mainRow = row;

    // Continues to run if column value is within the parameter(844)
    for (int mainCol = 0; mainCol < NUMCOLS - 1; mainCol++)
    {
        int straight = gridMap[mainRow][mainCol + 1];
        int original = gridMap[mainRow][mainCol];

        al_draw_filled_rectangle(mainCol, mainRow, mainCol + 1, mainRow + 1, colour);

        // Checks if it is not row 0 or row 480
        // All the rows in between 0 and 480 are able to go in all three directions
        // If values are the same it is randomly selected otherwise the smallest one is selected
        if ((mainRow - 1 >= 0) && (mainRow + 1 < NUMROWS))
        {
            int up = gridMap[mainRow - 1][mainCol + 1];
            int down = gridMap[mainRow + 1][mainCol + 1];

            if (up < straight && up < down)
            {
                mainRow--;
                totalElevation += abs(up - original);
            }
            else if (down == up && down < straight)
            {
                totalElevation += abs(down - original);
                if (rand() % 2 == 0)
                {
                    mainRow++;
                }
                else
                {
                    mainRow--;
                }
            }
            else if (down < up && down < straight)
            {
                mainRow++;
                totalElevation += abs(down - original);
            }
            else if (straight < up && straight < down)
            {
                totalElevation += abs(straight - original);
            }
            else if (straight == up && straight < down)
            {
                totalElevation += abs(straight - original);
            }
            else if (straight == down && straight < up)
            {
                totalElevation += abs(straight - original);
            }
            else if (straight == up && straight == down)
            {
                totalElevation += abs(straight - original);
            }
        }
        // Checks if it is row 480
        // Row 480 can only go straight and up diagonal
        // If values are the same it is randomly selected otherwise the smallest one is selected
        else if (mainRow + 1 == NUMROWS)
        {
            int up = gridMap[mainRow - 1][mainCol + 1];
            if (up < straight)
            {
                mainRow--;
                totalElevation += abs(up - original);
            }
            else if (straight < up)
            {
                totalElevation += abs(straight - original);
            }
            else if (straight == up)
            {
                totalElevation += abs(straight - original);
            }
        }
        // Checks if it is row 0
        // Row 480 can only go straight and down diagonal
        // If values are the same it is randomly selected otherwise the smallest one is selected
        else if (mainRow == 0)
        {
            int down = gridMap[mainRow + 1][mainCol + 1];
            if (down < straight)
            {
                mainRow++;
                totalElevation += abs(down - original);
            }
            else if (straight < down)
            {
                totalElevation += abs(straight - original);
            }
            else if (straight == down)
            {
                totalElevation += abs(straight - original);
            }
        }
    }
    return totalElevation;
}

// Takes in mountain matrix data and line colour as inputs
// Calls the drawing and calculation function(for the lowest elevation algorithm) for each row
// Returns the lowest elevation change index
int indexOfLowestElevPath(apmatrix<short> &gridMap, ALLEGRO_COLOR colour)
{
    int index;
    int lElevation = INT_MAX;

    for (int i = 0; i < NUMROWS; i++){
        int curElev = lowestElevation(gridMap, i, colour);
        // Takes the smallest elevation change for each row and keeps the index location
        if (curElev < lElevation){
            lElevation = curElev;
            index = i;
        }
    }
    return index;
}

// Takes in mountain matrix data, starting row, and line colour as inputs
// From a starting row it draws the path through using the 5 direction greedy algorithm
// Returns total elevation change of the path
int fiveFowardLocations(apmatrix<short> &gridMap, int row, ALLEGRO_COLOR colour)
{
    srand(time(0));

    int totalElevation = 0;
    int mainRow = row;
    int mainCol = 0;
    int run = true;

    while(run)
    {
        // Reads in the value of the matrix into a variable
        // Once done so it changes the value of that location to SHRT_MAX so it prevents back tracking
        int original = gridMap[mainRow][mainCol];
        gridMap[mainRow][mainCol] = SHRT_MAX;

        // Checks if column is equal to 844, if so it stops running
        if(mainCol + 1 == NUMCOLS)
        {
            break;
        }

        al_draw_filled_rectangle(mainCol, mainRow, mainCol + 1, mainRow + 1, colour);

        // Checks if it is row 0
        // Row 0 can only go straight, down, and down diagonal
        // If values are the same it is randomly selected otherwise the smallest one is selected
        if (mainRow == 0)
        {
            int straight = abs(gridMap[mainRow][mainCol + 1] - original);
            int downFrontDiagonal = abs(gridMap[mainRow + 1][mainCol + 1] - original);
            int down = abs(gridMap[mainRow + 1][mainCol] - original);

            if (downFrontDiagonal < straight && downFrontDiagonal < down)
            {
                mainRow++;
                mainCol++;
                totalElevation += downFrontDiagonal;
            }
            else if (straight < downFrontDiagonal && straight < down)
            {
                mainCol++;
                totalElevation += straight;
            }
            else if (down < straight && down < downFrontDiagonal)
            {
                mainRow++;
                totalElevation += down;
            }
            else if (down == straight && down < downFrontDiagonal)
            {
                totalElevation += straight;
                mainCol++;
            }
            else if (down == downFrontDiagonal && down < straight)
            {
                totalElevation += downFrontDiagonal;
                mainCol++;
                mainRow++;
            }
            else if (straight == downFrontDiagonal && straight < down)
            {
                totalElevation += straight;
                mainCol++;
            }
            else
            {
                totalElevation += straight;
                mainCol++;
            }
        }
        // Checks if it is in between row 0 and 480
        // Rows in between 0 and 480 can go straight, up diagonal, down diagonal, up, and down
        // If values are the same it is randomly selected otherwise the smallest one is selected
        else if (mainRow != 0 && mainRow != NUMROWS - 1)
        {
            int straight = abs(gridMap[mainRow][mainCol + 1] - original);
            int upFrontDiagonal = abs(gridMap[mainRow - 1][mainCol + 1] - original);
            int downFrontDiagonal = abs(gridMap[mainRow + 1][mainCol + 1] - original);
            int up = abs(gridMap[mainRow - 1][mainCol] - original);
            int down = abs(gridMap[mainRow + 1][mainCol] - original);

            if (straight < upFrontDiagonal && straight < downFrontDiagonal && straight < up && straight < down)
            {
                mainCol++;
                totalElevation += straight;
            }
            else if (upFrontDiagonal < straight && upFrontDiagonal < up && upFrontDiagonal < down && upFrontDiagonal < downFrontDiagonal)
            {
                mainCol++;
                mainRow--;
                totalElevation += upFrontDiagonal;
            }
            else if (downFrontDiagonal < straight && downFrontDiagonal < up && downFrontDiagonal < down && downFrontDiagonal < upFrontDiagonal)
            {
                mainCol++;
                mainRow++;
                totalElevation += downFrontDiagonal;
            }
            else if (up < straight && up < down && up < upFrontDiagonal && up < downFrontDiagonal)
            {
                mainRow--;
                totalElevation += up;
            }
            else if (down < straight && down < up && down < upFrontDiagonal && down < downFrontDiagonal)
            {
                mainRow++;
                totalElevation += down;
            }
            else if (straight == up && straight < upFrontDiagonal && straight < downFrontDiagonal && straight < down)
            {
                totalElevation += straight;
                mainCol++;
            }
            else if (straight == down && straight < upFrontDiagonal && straight < downFrontDiagonal && straight < up)
            {
                totalElevation += straight;
                mainCol++;
            }
            else if (straight == upFrontDiagonal && straight < down && straight < downFrontDiagonal && straight < up)
            {
                totalElevation += straight;
                mainCol++;
            }
            else if (straight == downFrontDiagonal && straight < upFrontDiagonal && straight < up && straight < down)
            {
                totalElevation += straight;
                mainCol++;
            }
            else if (up == down && up < upFrontDiagonal && up < downFrontDiagonal && up < straight)
            {
                totalElevation += up;
                if (rand() % 2 == 0)
                {
                    mainRow--;
                }
                else
                {
                    mainRow++;
                }
            }
            else if (up == upFrontDiagonal && up < straight && up < down && up < downFrontDiagonal)
            {
                totalElevation += upFrontDiagonal;
                mainCol++;
                mainRow--;
            }
            else if (up == downFrontDiagonal && up < straight && up < down && up < upFrontDiagonal)
            {
                totalElevation += downFrontDiagonal;
                mainCol++;
                mainRow++;
            }
            else if (down == upFrontDiagonal && down < up && down < straight && down < downFrontDiagonal)
            {
                totalElevation += upFrontDiagonal;
                mainCol++;
                mainRow--;
            }
            else if (down == downFrontDiagonal && down < up && down < straight && down < upFrontDiagonal)
            {
                totalElevation += downFrontDiagonal;
                mainCol++;
                mainRow++;
            }
            else if (upFrontDiagonal == downFrontDiagonal && upFrontDiagonal < up && upFrontDiagonal < down && upFrontDiagonal < straight)
            {
                totalElevation += upFrontDiagonal;
                if (rand() % 2 == 0)
                {
                    mainCol++;
                    mainRow--;
                }
                else
                {
                    mainCol++;
                    mainRow++;
                }
            }
            else if (straight == up && straight == down && straight < upFrontDiagonal && straight < downFrontDiagonal)
            {
                totalElevation += straight;
                mainCol++;
            }
            else if (straight == up && straight == upFrontDiagonal && straight < downFrontDiagonal && straight < down)
            {
                totalElevation += straight;
                mainCol++;
            }
            else if (straight == up && straight == downFrontDiagonal && straight < upFrontDiagonal && straight < down)
            {
                totalElevation += straight;
                mainCol++;
            }
            else if (straight == down && straight == upFrontDiagonal && straight < downFrontDiagonal && straight < up)
            {
                totalElevation += straight;
                mainCol++;
            }
            else if (straight == down && straight == downFrontDiagonal && straight < up && straight < upFrontDiagonal)
            {
                totalElevation += straight;
                mainCol++;
            }
            else if (straight == upFrontDiagonal && straight == downFrontDiagonal && straight < up && straight < down)
            {
                totalElevation += straight;
                mainCol++;
            }
            else if (up == down && up == upFrontDiagonal && up < straight && up < downFrontDiagonal)
            {
                totalElevation += upFrontDiagonal;
                mainCol++;
                mainRow--;
            }
            else if (up == down && up == downFrontDiagonal && up < straight && up < upFrontDiagonal)
            {
                totalElevation += downFrontDiagonal;
                mainCol++;
                mainRow++;
            }
            else if (up == upFrontDiagonal && up == downFrontDiagonal && up < straight && up < down)
            {
                totalElevation += upFrontDiagonal;
                int rNum = rand() % 2;
                if (rNum == 0)
                {
                    mainCol++;
                    mainRow++;
                }
                else if (rNum == 1)
                {
                    mainCol++;
                    mainRow--;
                }
            }
            else if (down == upFrontDiagonal && down == downFrontDiagonal && down < straight && down < up)
            {
                totalElevation += upFrontDiagonal;
                int rNum = rand() % 2;
                if (rNum == 0)
                {
                    mainCol++;
                    mainRow++;
                }
                else if (rNum == 1)
                {
                    mainCol++;
                    mainRow--;
                }
            }
            ////
            else if (straight == up && straight == down && straight == upFrontDiagonal && straight < downFrontDiagonal)
            {
                totalElevation += straight;
                mainCol++;
            }
            else if (straight == up && straight == down && straight == downFrontDiagonal && straight < upFrontDiagonal)
            {
                totalElevation += straight;
                mainCol++;
            }
            else if (straight == up && straight == upFrontDiagonal && straight == downFrontDiagonal && straight < down)
            {
                totalElevation += straight;
                mainCol++;
            }
            else if (straight == down && straight == upFrontDiagonal && straight == downFrontDiagonal && straight < up)
            {
                totalElevation += straight;
                mainCol++;
            }
            else if (up == down && up == upFrontDiagonal && up == downFrontDiagonal && up < straight)
            {
                totalElevation += upFrontDiagonal;
                int rNum = rand() % 2;
                if (rNum == 0)
                {
                    mainCol++;
                    mainRow--;
                }
                else if (rNum == 1)
                {
                    mainCol++;
                    mainRow++;
                }
            }
            else if (straight == up && straight == down && straight == upFrontDiagonal && straight == downFrontDiagonal)
            {
                totalElevation += straight;
                mainCol++;
            }
        }
        // Checks if it is row 480
        // Row 480 can only go straight, up, and up diagonal
        // If values are the same it is randomly selected otherwise the smallest one is selected
        else if (mainRow + 1 == NUMROWS)
        {
            int straight = abs(gridMap[mainRow][mainCol + 1] - original);
            int upFrontDiagonal = abs(gridMap[mainRow - 1][mainCol + 1] - original);
            int up = abs(gridMap[mainRow - 1][mainCol] - original);
            if (upFrontDiagonal < straight && upFrontDiagonal < up)
            {
                mainCol++;
                mainRow--;
                totalElevation += upFrontDiagonal;
            }
            else if (straight < upFrontDiagonal && straight < up)
            {
                mainCol++;
                totalElevation += straight;
            }
            else if (up < straight && up < upFrontDiagonal)
            {
                mainRow--;
                totalElevation += up;
            }
            else if (up == straight && up < upFrontDiagonal)
            {
                totalElevation += straight;
                mainCol++;
            }
            else if (up == upFrontDiagonal && up < straight)
            {
                totalElevation += upFrontDiagonal;
                mainCol++;
                mainRow--;
            }
            else if (straight == upFrontDiagonal && straight < up)
            {
                totalElevation += straight;
                mainCol++;
            }
            else
            {
                totalElevation += straight;
                mainCol++;
            }
        }
    }
    return totalElevation;
}

// Takes in mountain matrix data and line colour as inputs
// Calls the drawing and calculation function(for the 5 direction greedy algorithm) for each row
// Returns the lowest elevation change index
int fiveFowardLocationsIndex(apmatrix<short> &gridMap, ALLEGRO_COLOR colour)
{
    apmatrix<short> copyOfGridMap(NUMROWS, NUMCOLS, 0);

    int index;
    int lElevation = INT_MAX;

    for (int i = 0; i < NUMROWS; i++)
    {
        // Reassigns the values of the colardo.dat to the mountain matrix
        // This is necessary because after the algorithm goes to a location in matrix the value is changed to SHRT_MAX so it can not back track
        copyGridMap(gridMap, copyOfGridMap);

        int curElev = fiveFowardLocations(copyOfGridMap, i, colour);
        // Takes the smallest elevation change for each row and keeps the index location
        if (curElev < lElevation){
            lElevation = curElev;
            index = i;
        }
    }
    return index;
}

// Takes in mountain matrix data and copy of mountain matrix data as inputs
// Assigns values of the mountain matrix to the copy of mountain matrix
// Does not return anything
void copyGridMap(const apmatrix<short> &gridMap, apmatrix<short> &copyOfGridMap)
{
    for(int j = 0; j < NUMROWS; j++)
    {
        for(int k = 0; k < NUMCOLS; k++)
        {
            copyOfGridMap[j][k] = gridMap[j][k];
        }
    }
}

// Takes in file name character array and mountain matrix data as inputs
// Reassigns values of the coloardo.dat into the mountain matrix
// This function is necessary as we change the values of the mountain matrix to SHRT_MAX once used.
// However, only after we draw the best path for this algorithm there is no place where we reset the values back to normal.
// Does not return anything
void reDraw(const char fileName[], apmatrix<short> &gridMap)
{
    ifstream infile("Colorado.dat");
    if(!infile.is_open())
    {
        cout << "File not found!";
    }

    for(int i = 0; i < NUMROWS; i++)
    {
        for(int j = 0; j < NUMCOLS; j++)
        {
            infile >> gridMap[i][j];
        }
    }
}
