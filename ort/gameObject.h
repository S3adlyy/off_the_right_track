#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "fonctions.h"
#include "scrolling.h"
#include "player.h"

typedef struct{
	Image img;
	SDL_Rect clip[3];
	int frame;
	Uint32 lastframetime;
	Mix_Chunk *sfx;
	int job; //Game item (adds to objective) | Enigma (When the player interacts with it, an enigma takes place) // 1- enigma | 2- game item
	int active;//1- yes | 0- No => dont display it anymore and maybe remove the game object from the array

}GameObject;



void init_gameObject(GameObject *obj, int x, int y, char filename[], int job);
void animate_gameObjects(GameObject *array, int n, Uint32 currenttime);
int update_gameObject(GameObject *array, int *n, Player p, Camera camera);
void display_gameObjects(GameObject *array, int n, Camera camera,SDL_Surface *ecran,  Uint32 currenttime);
void delete_gameObject(GameObject *array, int *n, int i) ;

int checkCollisionGO(Player p, SDL_Rect b, Camera camera);
void liberer_gameObjects(GameObject *array, int n);


#endif /* GAMEOBJECT_H */

