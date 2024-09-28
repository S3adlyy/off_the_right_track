#ifndef ENEMY_H
#define ENEMY_H
#include <SDL/SDL.h>
#include <SDL/SDL_image.h> 
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "fonctions.h"
#include "scrolling.h"
#include "player.h"
#define ENEMY_IDLE 0
#define ENEMY_RIGHT 1
#define ENEMY_LEFT 2
#define ENEMY_DEAD 3

typedef struct{
	SDL_Rect pos, posInit;
	int hp;
	SDL_Rect rightframes[3][4], leftframes[3][4];//0- walk 1- run/fight 2- death
	Image spritesheets[6];
	int lastframetime;
	Uint32 lastdamagetime;
	int status, frame, laststatus;
	int isFighting, punched;//punched variable is to assure that the enemy punches the player only once at a time
	int maxrange;
	int enemyReturnTimer, fightTimer;
	SDL_Rect hpbarprogress, hpbar;
	


}Enemy;


void init_enemy(Enemy *e, int x, int y);
void afficher_enemy(Enemy e, SDL_Surface *ecran, Camera camera);


void move_enemy(Enemy *e, Player p, Uint32 dt, Camera camera);
int check_enemy(Player *p, Enemy *e, Camera camera, Mix_Chunk *health_hit_sfx);
void mvt_enemy(Enemy *e, Player *p);

void ai_enemy(Player *p, Enemy *e, Camera camera, Uint32 currentTime);
void animer_enemy(Enemy *e,Uint32 currentTime, Uint32 dt);
int detect_collision(Player *p, Enemy *e, Camera camera);
void liberer_enemy(Enemy e);



































#endif /* ENEMY_H */

