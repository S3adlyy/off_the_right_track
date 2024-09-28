#include <SDL/SDL.h>//inclure SDL.h  pour permettre au programme d'être portable sur tous les systèmes
#include <SDL/SDL_image.h> //Pour manipuler des images ayants des types autre que b
#include <SDL/SDL_mixer.h>//Pour manipuler de l’audio.
#include <SDL/SDL_ttf.h>
#include "fonctions.h"
#include "main.h"
#include "settings.h"
#include "newgame.h"
#include "constants.h"
#include "level1.h"
#include "level2.h"
#include "level3.h"

	


int main(){
	int quit=0;
	int page=1;
	SDL_Surface *ecran=NULL;
	
	SDL_Init( SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER);
	TTF_Init();
	/*initialisation de SDL_Video */
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER) < 0)
	{
		printf("Echec d'initialisation de SDL : %s\n", SDL_GetError());
		return 1;
	
	}
	
	SDL_WM_SetCaption("OFF THE RIGHT TRACK", NULL);
	ecran = SDL_SetVideoMode(SCREEN_W, SCREEN_H,  32,SDL_HWSURFACE | SDL_DOUBLEBUF); 
	if ( ecran == NULL )
	{
		fprintf(stderr, "Echec de creation de la fenetre de %dx%d: %s.\n",SCREEN_W, SCREEN_H, SDL_GetError());
		return 1;
	}
	printf("here\n");
	while(!quit){
		printf("page: %d \n", page);
		switch(page){
			case 0: quit=1;break;
			case 1: menu(ecran, &page);break;
			case 2: game_level1(ecran, &page);break;
			case 3: game_level2(ecran, &page);break;//change to level2
			case 4: game_level3(ecran, &page);break;
			default:break;
		
		
		}
	
	
	}
	TTF_Quit();
	SDL_Quit();






}
