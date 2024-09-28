#ifndef PLAYER_H
#define PLAYER_H
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "fonctions.h"
#include "constants.h"
#define PLAYER_IDLE 0
#define PLAYER_RIGHT 1
#define PLAYER_LEFT 2
#define PLAYER_DEAD 3
#define GRAVITY 50
#define JUMP_FORCE 50

typedef struct{
	SDL_Rect pos, prev_pos;//prev_pos is used for collision 
	int playerW, playerH;//change it in every spritesheet animation
	double vitesse, vitesse_y, acceleration, acceleration_y, lastdx, lastdy, dashTimer;
	int status, laststatus;//right or left or idle
	int lastjumpY;
	int isRunning, isJumping, isGrounded, cameraIsScrolling, isDashing;//0. false || 1. true
	int hp, stamina; //max hp=4 | max stamina=100   --maybe make jumping equations related to stamina
	int frame, max_frames, max_rows, currentRow;//for spritesheet 2d array
	int staminaTimer;
	Uint32 lastframetime;
	int bc, tc, lc, rc;//collision logic bottom-top-left-right | 1- colliding\ 0- not colliding
	int isFighting, isGuarding;
	SDL_Rect posRel;
	SDL_Rect cameraPos;
	SDL_Rect **clip;//spritesheet clipped positions (matrice/2d array)
	Image deathscreen;
	Text deathyes_txt, deathno_txt, deathscore_txt;
	Mix_Chunk *punch_sfx;
	Uint32 lastdamaged;
}Player;


void init_player(Player *p, int x, int y, int max_frames, int max_rows);
void init_player_spritesheet(Player *p, Image spritesheets[]);
void apply_gravity(Player *p, Uint32 dt) ;
void move_player(Player *p, Uint32 dt);
void jump(Player *p, double dt, int posAbs_x, int posAbs_y);
int currentPlayerAnimation(Player p1);
void display_player(Player p, SDL_Surface* ecran, Image spritesheet, SDL_Rect* clip);
void handlePlayerKeyDownEvent(Player *p1, SDL_Event event,Uint8 keystate[]);
void handlePlayerKeyUpEvent(Player *p1, SDL_Event event, Uint8 keystate[]);
void handlePlayerKeyHeldEvent(Player *p1, Uint8 keystate[], Uint32 dt);
void handlePlayerAnimation(Player *p1, Uint32 currentTime, Uint32 dt);
void updatePlayerHW(Player *p, SDL_Rect currentClip); //for collision behaviour
void displayStaminaBar(Player p, Image staminaimg, SDL_Surface *screen);
void liberer_player(Player p);

/*

steps to integrate
1- init player
2- init spritesheet
3-keystate function call
4-in switch event case -> keydown case -> handlePlayerKeydownEvent
5-in switch event case -> keyup case -> handlePlayerKeyUpEvent
6- after switch case-> calculate dt and currenttime -> call handleplayerkeyheldevent THEN handle player animation
7- call displayplayer func



*/

#endif
