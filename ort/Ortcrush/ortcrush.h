#ifndef ORTCRUSH_H
#define ORTCRUSH_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h> 
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "../fonctions.h"


#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define COLUMN_COUNT 9
#define ROW_COUNT 9
#define CANDY_COUNT 6

typedef struct {
    SDL_Surface* image;
    int type;
} Candy;

void initializeGrid(Candy grid[COLUMN_COUNT][ROW_COUNT]);
void renderGrid2(SDL_Surface* screen, Candy grid[COLUMN_COUNT][ROW_COUNT], int selectedCol1, int selectedRow1, int selectedCol2, int selectedRow2, SDL_Surface *gridcell, SDL_Surface *gridcellselected, Image ortcrushbg, Image starwhite1,  Image starwhite2, Image starwhite3, Text timertxt, Text scoretxt, char timertotext[], char scoretotext[], Image stargold1, Image stargold2, Image stargold3,int score, int maxScore,float totalProgress,SDL_Rect progressBarRect, SDL_Rect progressBarScore,float currentProgress );
void swapCandies(Candy* candy1, Candy* candy2);
int checkMatches(Candy grid[COLUMN_COUNT][ROW_COUNT], int *score, int maxScore, float *totalProgress, SDL_Surface *screen, Image particleT[], Text scorePopUp, SDL_Surface *gridcell, SDL_Surface *gridcellselected, Image ortcrushbg, Image starwhite1,  Image starwhite2, Image starwhite3, Text timertxt, Text scoretxt, char timertotext[], char scoretotext[], Image stargold1, Image stargold2, Image stargold3,SDL_Rect progressBarRect, SDL_Rect *progressBarScore,float *currentProgress);
void shiftCandies2(SDL_Surface* screen);
void shiftCandies(Candy grid[COLUMN_COUNT][ROW_COUNT], SDL_Surface *gridcell, SDL_Surface *gridcellselected, SDL_Surface *screen, Image ortcrushbg, Image starwhite1,  Image starwhite2, Image starwhite3, Text timertxt, Text scoretxt, char timertotext[], char scoretotext[], Image stargold1, Image stargold2, Image stargold3,int score, int maxScore,float totalProgress,SDL_Rect progressBarRect, SDL_Rect progressBarScore,float currentProgress  );
void fillTopRow(Candy grid[COLUMN_COUNT][ROW_COUNT]);
int handleInput(SDL_Event event,Candy grid[COLUMN_COUNT][ROW_COUNT], int* selectedColumn, int* selectedRow, int* secondColumn, int* secondRow) ;
void swapSelectedCandies(Candy grid[COLUMN_COUNT][ROW_COUNT], int selectedColumn, int selectedRow, int newColumn, int newRow);
void afficherGrid(Candy grid[COLUMN_COUNT][ROW_COUNT]);
void animateFallingCandies(Candy grid[COLUMN_COUNT][ROW_COUNT], int colX, int endRow, int startRow, SDL_Surface *gridcell, SDL_Surface *gridcellselected, SDL_Surface *screen, Image ortcrushbg, Image starwhite1,  Image starwhite2, Image starwhite3, Text timertxt, Text scoretxt, char timertotext[], char scoretotext[] , Image stargold1, Image stargold2, Image stargold3,int score, int maxScore,float totalProgress,SDL_Rect progressBarRect, SDL_Rect progressBarScore,float currentProgress);
void fillEmptyCol(Candy grid[COLUMN_COUNT][ROW_COUNT], SDL_Surface *gridcell, SDL_Surface *gridcellselected, SDL_Surface *screen, Image ortcrushbg, Image starwhite1,  Image starwhite2, Image starwhite3, Text timertxt, Text scoretxt, char timertotext[], char scoretotext[], Image stargold1, Image stargold2, Image stargold3,int score, int maxScore,float totalProgress,SDL_Rect progressBarRect, SDL_Rect progressBarScore,float currentProgress );
void freeGrid(Candy grid[COLUMN_COUNT][ROW_COUNT]);
void initParticleFX(Image particleT[]);
void displayParticleFXAnimationHorizontal(SDL_Surface *screen,  Image particleT[], int row, int col1, int colf ,Text scorePopUp, int scoretoadd, SDL_Surface *gridcell, SDL_Surface *gridcellselected, Image ortcrushbg, Image starwhite1,  Image starwhite2, Image starwhite3, Text timertxt, Text scoretxt, char timertotext[], char scoretotext[], Image stargold1, Image stargold2, Image stargold3,int score, int maxScore,float totalProgress,SDL_Rect progressBarRect, SDL_Rect *progressBarScore,float *currentProgress, Candy grid[COLUMN_COUNT][ROW_COUNT]);
void displayParticleFXAnimationVertical(SDL_Surface *screen,  Image particleT[], int col, int row1, int rowf, Text scorePopUp, int scoretoadd, SDL_Surface *gridcell, SDL_Surface *gridcellselected, Image ortcrushbg, Image starwhite1,  Image starwhite2, Image starwhite3, Text timertxt, Text scoretxt, char timertotext[], char scoretotext[], Image stargold1, Image stargold2, Image stargold3,int score, int maxScore,float totalProgress,SDL_Rect progressBarRect, SDL_Rect progressBarScore,float currentProgress, Candy grid[COLUMN_COUNT][ROW_COUNT]);
const int getRandomInt(int min, int max) ;

#endif 
