#ifndef MEMORYGAME_H
#define MEMORYGAME_H
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>


void init_memorygame_spritesheet(SDL_Rect T[]);//pass: SDL_Rect T[6] as param
void init_memorygame(Image T[]);//pass: Image T[6] as param
void init_memorygame_card_frames(int cardsframes[]);
void afficher_memorygame(SDL_Rect T[], Image T2[], SDL_Surface *ecran, int cardsframes[]);
void init_memorygame_answers(int memorygame_answers[]);
int check_memorygame(int cardsframes[], int memorygame_answers[], int selectedcard1, int selectedcard2);
int rewardPlayer(SDL_Surface *ecran, SDL_Surface *cursorImage4,SDL_Rect cur_pos, float oldCurPosX, float oldCurPosY, int *r);




#endif
