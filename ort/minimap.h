#ifndef MINIMAP_H
#define MINIMAP_H
#define MINIMAP_REDIMENSION 0.08
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

#include "fonctions.h"
#include "constants.h"
#include "player.h"
#include "scrolling.h"

typedef struct {
	Image img_minimap, img_miniplayer;
	SDL_Rect camera;

}Minimap;

typedef struct{
	Image img_indicator;
	char type[10];//maybe remove
	SDL_Rect pos;
	SDL_Rect frame_pos;
	int max_frames, k;
}Indicator;

void init_minimap(Minimap *M, int x, int y, SDL_Rect playerPos, char filepath[]);
void init_minimapVertical(Minimap *M, int x, int y, SDL_Rect playerPos, char filepath[]);
void afficher_minimap(Minimap M, SDL_Surface *screen) ;
void liberer_minimap(Minimap M);
Indicator add_minimap_indicator(char type[], int x, int y, int max_frames,Minimap M);
void animate_minimap_indicator(Indicator *ind,SDL_Surface *screen,int animate);
void updateMiniplayer(Minimap *M, Player p, Camera cam);
void updateMiniplayerVertical(Minimap *M, Player p, Camera cam);
void liberer_indicator(Indicator ind);
//void animate_miniplayer(Player p, Minimap *M);

#endif /* NAME_H */

