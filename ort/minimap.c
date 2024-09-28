#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "minimap.h"

#include <string.h>


void init_minimap(Minimap *M, int x, int y, SDL_Rect playerPos, char filepath[]){//change x y when implementing saving feature
	//if 2 players: either allocate another image for the 2nd player (array of players) and display it in the same minimap or call the minimap function for each player 
	init_image(&M->img_minimap, filepath, (float)x, (float)y);
	
	//init miniplayer pos
	init_image(&M->img_miniplayer, "miniplayertest.png", (float)(x + playerPos.x*MINIMAP_REDIMENSION), (float)(M->img_minimap.img->h + y));
	
	//make position more precise
	M->img_miniplayer.pos1.x -= M->img_miniplayer.img->w/3;
	
}

void init_minimapVertical(Minimap *M, int x, int y, SDL_Rect playerPos, char filepath[]){//change x y when implementing saving feature
	//if 2 players: either allocate another image for the 2nd player (array of players) and display it in the same minimap or call the minimap function for each player 
	init_image(&M->img_minimap, filepath, (float)x, (float)y);
	
	//init miniplayer pos
	init_image(&M->img_miniplayer, "miniplayertest.png", (float)(x + M->img_minimap.img->w+50), (float)(M->img_minimap.img->h + y));
	
	//make position more precise
	M->img_miniplayer.pos1.x -= M->img_miniplayer.img->w/3;
	
}

void afficher_minimap(Minimap M, SDL_Surface *screen) {
    // afficher minimap
   //afficher_sprite(M.img_minimap, screen, &M.camera);
   afficher_image(M.img_minimap, screen);
   //afficher_image(M.img_minimap, screen);
    // Blit the player indicator image onto the minimap
    afficher_image(M.img_miniplayer, screen);

}

void liberer_minimap(Minimap M){
	liberer_image(M.img_minimap);
	liberer_image(M.img_miniplayer);
}

Indicator add_minimap_indicator(char type[], int x, int y, int max_frames, Minimap M){
	//types: help/enigma/npc/challenge/etc..
	Indicator ind;
	x = M.img_minimap.pos1.x + x*MINIMAP_REDIMENSION;
	y = M.img_minimap.pos1.y + y*MINIMAP_REDIMENSION;
	if(strcmp(type, "npc")==0){
		init_image(&ind.img_indicator,"npc_minimap_indicator.png", x, y);
	}
	else if(strcmp(type, "enigma")==0){
		init_image(&ind.img_indicator,"enigma_minimap_indicator.png", x, y);
	}
	ind.k=1;
	ind.pos.x = x;
	ind.pos.y= y;
	ind.frame_pos.x = 0;
	ind.frame_pos.y=0;
	ind.frame_pos.w=ind.img_indicator.img->w;
	ind.frame_pos.h=ind.img_indicator.img->h/max_frames;
	ind.max_frames = max_frames;
	strcpy(ind.type, type);
	return ind;

}
void animate_minimap_indicator(Indicator *ind, SDL_Surface *screen, int animate){
	if(animate){
		int unit = ind->img_indicator.img->h/ind->max_frames;
		ind->frame_pos.y += ind->k*unit;
		if(ind->frame_pos.y>= ind->img_indicator.img->h - unit || ind->frame_pos.y==0)
			ind->k*=-1;
		//printf("unit: %d || frame pos y: %d\n", unit, ind->frame_pos.y);

	}
	//afficher_sprite(ind->img_indicator, screen, &ind->frame_pos);   //reactivate this later
	

}
void updateMiniplayer(Minimap *M, Player p, Camera cam){
	//if(p.pos.x != M->img_minimap.pos1.x + (cam.pos.x + p.pos.x) * MINIMAP_REDIMENSION - M->img_miniplayer.img->w/3)
		 M->img_miniplayer.pos1.x = M->img_minimap.pos1.x + (cam.pos.x + p.pos.x) * MINIMAP_REDIMENSION - M->img_miniplayer.img->w/3;
	printf("miniplayer pos x: %d\n", M->img_miniplayer.pos1.x); 
	printf("player pos x: %d | player pos y: %d\n", p.pos.x, p.pos.y); 
	/*if(p.pos.y != M->img_miniplayer.pos1.y)
		M->img_miniplayer.pos1.y = M->img_minimap.pos1.y + p.pos.y * MINIMAP_REDIMENSION;*/
}

void updateMiniplayerVertical(Minimap *M, Player p, Camera cam){
	//if(p.pos.x != M->img_minimap.pos1.x + (cam.pos.x + p.pos.x) * MINIMAP_REDIMENSION - M->img_miniplayer.img->w/3)
		 M->img_miniplayer.pos1.y = M->img_minimap.pos1.y + M->img_minimap.pos1.h -  (cam.pos.y + p.pos.y) * MINIMAP_REDIMENSION;// - M->img_miniplayer.img->w/3;
	printf("miniplayer pos x: %d\n", M->img_miniplayer.pos1.x); 
	printf("player pos x: %d | player pos y: %d\n", p.pos.x, p.pos.y); 
	/*if(p.pos.y != M->img_miniplayer.pos1.y)
		M->img_miniplayer.pos1.y = M->img_minimap.pos1.y + p.pos.y * MINIMAP_REDIMENSION;*/
}

void liberer_indicator(Indicator ind){
	liberer_image(ind.img_indicator);
}


/*void animate_miniplayer(Player p, Minimap *M){
	M->img_miniplayer.pos1.x = M->img_minimap.pos1.x + p.pos.x*MINIMAP_REDIMENSION;
	M->img_miniplayer.pos1.y = M->img_minimap.pos1.y + p.pos.y*MINIMAP_REDIMENSION;

}*/

