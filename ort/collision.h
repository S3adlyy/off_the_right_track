#ifndef COLLISION_H
#define COLLISION_H

#include "fonctions.h"
#include "constants.h"
//#include "player.h"
#include "scrolling.h"


int CheckCollision(Image *mask, int x, int y);
SDL_Color GetPixel(SDL_Surface *pSurface,int x,int y);
void CollisionDetection(Image *mask, Player *p, Camera camera);//USE THIS IN MAIN
int FindNearestValidX(Image *mask, Player *p, Camera camera, int x , int y, int dir);
int FindNearestValidY(Image *mask, Player *p, Camera camera, int x, int y, int dir);


//bounding box colision sys
void handlePlayerCollision(Player *player, Image *obstacles, int numObstacles, Camera camera, SDL_Surface *ecran);
int checkCollision(SDL_Rect a, SDL_Rect b);
void updateObstaclesWithCamera(Image* obstacles, int num_obstacles,Camera camera);

//pixel per v2
int CollisionDetectionLeft(Image *mask, Player *p, Camera camera);
int CollisionDetectionBottom(Image *mask, Player *p, Camera camera);
int CollisionDetectionTop(Image *mask, Player *p, Camera camera);
int CollisionDetectionRight(Image *mask, Player *p, Camera camera);


#endif /* COLLISION_H */

