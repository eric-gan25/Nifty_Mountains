#include <apmatrix.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>

#define NUMROWS 480
#define NUMCOLS 844
#define GREEN al_map_rgb(0, 255, 0)
#define RED al_map_rgb(255, 0, 0)
#define BLUE al_map_rgb(0, 0, 255)
#define YELLOW al_map_rgb(255, 255, 0)
#define PINK al_map_rgb(238, 144, 144)
#define PURPLE al_map_rgb(1600, 26, 140)
#define ORANGE al_map_rgb(255, 165, 0)

//Prototypes for functions
//Initialize Allegro prototype
int initializeAllegro(int width, int height, const char title[], ALLEGRO_DISPLAY *&display);

//Base requirement function prototypes
short findMax(apmatrix<short> &gridMap);
short findMin(apmatrix<short> &gridMap);
void drawMap(apmatrix<short> &gridMap);
int drawLowestElevPath(apmatrix<short> &gridMap, int row, ALLEGRO_COLOR colour);
int indexGreedy(apmatrix<short> &gridMap, ALLEGRO_COLOR colour);

//Above and beyond function prototypes
int lowestElevation(apmatrix<short> &gridMap, int row, ALLEGRO_COLOR colour);
int indexOfLowestElevPath(apmatrix<short> &gridMap, ALLEGRO_COLOR colour);
int fiveFowardLocations(apmatrix<short> &gridMap, int row, ALLEGRO_COLOR colour);
int fiveFowardLocationsIndex(apmatrix<short> &gridMap, ALLEGRO_COLOR colour);
void copyGridMap(const apmatrix<short> &gridMap, apmatrix<short> &copyOfGridMap);
void reDraw(const char fileName[], apmatrix<short> &gridMap);

