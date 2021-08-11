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

// Takes mountain matrix data as input
// Finds the max value in the matrix
// Returns the max value
short findMax(apmatrix<short> &gridMap)
{
    short maxVal = 0;
    for(int i = 0; i < NUMROWS; i++)
    {
        for(int j = 0; j < NUMCOLS; j++)
        {
            maxVal  = max(maxVal, gridMap[i][j]);
        }
    }
    return maxVal;
}

// Takes mountain matrix data as input
// Finds the min value in the matrix
// Returns the min value
short findMin(apmatrix<short> &gridMap)
{
    short minVal = SHRT_MAX;
    for(int i = 0; i < NUMROWS; i++)
    {
        for(int j = 0; j < NUMCOLS; j++)
        {
            minVal = min(minVal, gridMap[i][j]);
        }
    }
    return minVal;
}

// Takes mountain matrix data as input
// Draws the matrix as an elevation map
// Does not return anything
void drawMap(apmatrix<short> &gridMap)
{
    float scaleShade = 0;
    short maxVal = findMax(gridMap);
    short minVal = findMin(gridMap);

    int green[3] = {3, 70, 35};
    int blue[3] = {3, 8, 35};
    int colours[3];

    for(int i = 0; i < NUMROWS; i++)
    {
        for(int j = 0; j < NUMCOLS; j++)
        {
            scaleShade = float(gridMap[i][j] - minVal) / maxVal;

            for(int k = 0; k < 3; k++)
            {
                colours[k] = blue[k] + (green[k] - blue[k]) * scaleShade;
            }

            ALLEGRO_COLOR colour = al_map_rgb(colours[0], colours[1], colours[2]);
            al_draw_filled_rectangle(j, i, j+1, i+1, colour);
        }
    }
    al_flip_display();
}

// Takes mountain matrix data, starting row path, and line colour as inputs
// From a starting row it draws the best path through using the greedy method
// Returns total elevation change of the path
int drawLowestElevPath(apmatrix<short> &gridMap, int row, ALLEGRO_COLOR colour)
{
   srand(time(0));

   int totalElevation = 0;
   int mainRow = row;

   for (int mainCol = 0; mainCol < NUMCOLS - 1; mainCol++)
   {
       int straight = 0;
       int upDiagonal = INT_MAX;
       int downDiagonal = INT_MAX;

       al_draw_filled_rectangle(mainCol, mainRow, mainCol + 1, mainRow + 1, colour);

       // Going straight occurs on any row
       straight = abs(gridMap[mainRow][mainCol] - gridMap[mainRow][mainCol + 1]);

       // Going up diagonal only occurs when row does not equal 0
       if (mainRow - 1 >= 0)
       {
           upDiagonal = abs(gridMap[mainRow][mainCol] - gridMap[mainRow - 1][mainCol + 1]);
       }
       // Going down diagonal only occurs when row does not equal 480
       if (mainRow + 1 < NUMROWS)
       {
           downDiagonal = abs(gridMap[mainRow][mainCol] - gridMap[mainRow + 1][mainCol + 1]);
       }

       // If values are the same it is randomly selected otherwise the smallest one is selected
       if (upDiagonal < straight && upDiagonal < downDiagonal)
       {
           mainRow--;
           totalElevation += upDiagonal;
       }
       else if (downDiagonal < straight && downDiagonal < upDiagonal)
       {
           mainRow++;
           totalElevation += downDiagonal;
       }
       else if (straight < downDiagonal && straight < upDiagonal)
       {
           totalElevation += straight;
       }
       else if (downDiagonal == upDiagonal && downDiagonal < straight)
       {
           totalElevation += downDiagonal;
           if (rand() % 2 == 0)
           {
               mainRow++;
           }
           else
           {
               mainRow--;
           }
       }
       else if (straight == downDiagonal && straight < upDiagonal)
       {
           totalElevation += straight;
       }
       else if (straight == upDiagonal && straight < downDiagonal)
       {
           totalElevation += straight;
       }
       else if (straight == upDiagonal && straight == downDiagonal)
       {
           totalElevation += straight;
       }
    }
    return totalElevation;
}

// Takes in mountain matrix data and line colour as inputs
// Calls the drawing and calculation function(for the greedy algorithm) for each row
// Returns the lowest elevation change index
int indexGreedy(apmatrix<short> &gridMap, ALLEGRO_COLOR colour)
{
    int index;
    int lElevation = INT_MAX;

    for (int i = 0; i < NUMROWS; i++){
        int curElev = drawLowestElevPath(gridMap, i, colour);
        if (curElev < lElevation){
            lElevation = curElev;
            index = i;
        }
    }
    return index;
}

