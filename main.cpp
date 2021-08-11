 /*****************************************************************************
 *	Name:     Eric Gan                                                       *
 *	Date:     02/25/2020                                                     *
 *                                                                           *
 *	Description: Displays possible paths from one side of a mountain         *
 *               to another while looking at the smallest change in elevation*
 *  Known Issue: 5 Direction Greedy Algorithm takes more time to run the     *
 *               whole thing.                                                *
 *****************************************************************************/
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

int main(int argc, char *argv[])
{
    //Allegro initialization
    ALLEGRO_DISPLAY *display;
    initializeAllegro(NUMCOLS, NUMROWS, "GAN_Eric Nifty Mountains", display);

    //Assign the filename to a character array for it to be used in other functions
    const char fileName[15] = "Colorado.dat";

    //Opens the file and checks if the file is open
    ifstream infile("Colorado.dat");
    if(!infile.is_open())
    {
        cout << "File not found!";
        return 1;
    }

    //Initialize matrix
    apmatrix<short> mountain(NUMROWS, NUMCOLS, 0);

    //Read in the values from the text file into the matrix
    for(int i = 0; i < NUMROWS; i++)
    {
        for(int j = 0; j < NUMCOLS; j++)
        {
            infile >> mountain[i][j];
        }
    }

    //Initialize and assign values to variables
    int inputAction = 0;
    bool run = true;

    //Prints out title
    cout << "Nifty Mountains Assignment" << endl
         << "By Eric Gan" << endl << endl;

    //Loop will always run until run is changed to false
    while(run)
    {
        //Options for the user to choose is outputted to the console and the response is read in
        cout << "Actions(1-5):" << endl
             << "[1] Run through all paths for Greedy Algorithm." << endl
             << "[2] Run through all paths for Prioritizing Going Downhill Algorithm" << endl
             << "[3] Run through all paths for 5 Direction Greedy Algorithm" << endl
             << "[4] Run [1] and [2] and [3]" << endl
             << "[5] Exit" << endl << endl
             << "Enter a number to choose your action: ";
        cin >> inputAction;
        cout << endl;

        //
        if (inputAction >= 1 && inputAction <= 5)
        {
            //Draws map
            drawMap(mountain);
            al_flip_display();

            //If input is 1 it will run the Greedy Algorithm
            //If input is 2 it will run the Prioritizing Going Downhill Algorithm
            //If input is 3 it will run the 5 Direction Greedy Algorithm
            //If input is 4 it will run all algorithms
            //If input is 5 it will change run to false and exit the loop
            //If input is something else it will output invalid choice
            if (inputAction == 1)
            {
                int index1 = indexGreedy(mountain, RED);
                int elevationChange1 = drawLowestElevPath(mountain, index1, YELLOW);
                cout << "Red represents all possible paths and yellow represents the best path" << endl
                     << "Change in elevation for the yellow path: " <<  elevationChange1 << endl
                     << "Blue path starts at row: " << index1 << endl;
            }
            else if (inputAction == 2)
            {
                int index2 = indexOfLowestElevPath(mountain, PURPLE);
                int elevationChange2 = lowestElevation(mountain, index2, ORANGE);
                cout << "Purple represents all possible paths and orange represents the best path" << endl
                     << "Change in elevation for the orange path: " <<  elevationChange2 << endl
                     << "Yellow path starts at row: " << index2 << endl;
            }
            else if (inputAction == 3)
            {
                cout << "*************** NOTE THAT THIS ALGORITHM TAKES LONGER TO DRAW ***************" << endl << endl;
                int index3 = fiveFowardLocationsIndex(mountain, PINK);
                int elevationChange3 = fiveFowardLocations(mountain, index3, BLUE);
                reDraw(fileName, mountain);
                cout << "Pink represents all possible paths and blue represents the best path" << endl
                     << "Change in elevation for the blue path: " <<  elevationChange3 << endl
                     << "Yellow path starts at row: " << index3 << endl;
            }
            else if (inputAction == 4)
            {
                cout << "*************** NOTE THAT THESE ALGORITHMS TAKES LONGER TO DRAW ***************" << endl << endl;
                int index1 = indexGreedy(mountain, RED);
                int elevationChange1 = drawLowestElevPath(mountain, index1, YELLOW);
                cout << "Red represents all possible paths and yellow represents the best path" << endl
                     << "Change in elevation for the yellow path: " <<  elevationChange1 << endl
                     << "Yellow path starts at row: " << index1 << endl << endl;

                int index2 = indexOfLowestElevPath(mountain, PURPLE);
                int elevationChange2 = lowestElevation(mountain, index2, ORANGE);
                cout << "Purple represents all possible paths and orange represents the best path" << endl
                     << "Change in elevation for the orange path: " <<  elevationChange2 << endl
                     << "Orange path starts at row: " << index2 << endl << endl;
                int index3 = fiveFowardLocationsIndex(mountain, PINK);
                int elevationChange3 = fiveFowardLocations(mountain, index3, BLUE);
                 // The reDraw function is necessary as we change the values of the mountain matrix to SHRT_MAX once it is used.
                 // However, once we draw the best path for this algorithm we never reset the values back to normal.
                 // Thus, a function is needed to do so.
                reDraw(fileName, mountain);
                cout << "Pink represents all possible paths and blue represents the best path" << endl
                     << "Change in elevation for the blue path: " <<  elevationChange3 << endl
                     << "Blue path starts at row: " << index3 << endl;
            }
            else
            {
                run = false;
                cout << "Exiting Nifty Mountains" ;
            }
        }
        else
        {
            cout << "Invalid choice" << endl;
        }
        cout << endl ;
        al_flip_display();
    }
    //Closes display and file
    al_destroy_display(display);
    infile.close();
}
