#ifndef SCROLLING_H
#define SCROLLING_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "player.h"
#include "fonctions.h"




// Structure for camera
typedef struct {
    SDL_Rect pos;
    int directionX, directionY; // 1: Right, 2: Left | 1: Up, 2: Down
    int dx; // Horizontal scrolling step
    int dy; // Vertical scrolling step
    int initY;//camera always returns and is initialized to this position
    int smoothness; // smoothing factor for camera movement
} Camera;


void initCamera(Camera *camera, int startX, int startY, int screenWidth, int screenHeight, int smoothness);
void updateCamera(Camera *camera, Player *p, int screenWidth, int screenHeight, int bgWidth, int bgHeight) ;
void updateCameraVertical(Camera *camera, Player *p, int screenWidth, int screenHeight, int bgWidth, int bgHeight);
void scrollHorizontal(Camera *camera, int direction, int step);
void scrollVertical(Camera *camera, int direction, int step);
//void renderBackground(SDL_Surface *screen, Image background, Camera *camera) ;
void renderBackground(SDL_Surface *screen, Image background, Image *maskedBackground, Camera *camera, Player *p) ;

#endif /* SCROLLING_H */
