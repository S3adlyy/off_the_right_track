#ifndef TUTORIAL_H
#define TUTORIAL_H
#include <SDL/SDL.h>
#include <SDL/SDL_image.h> 
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "fonctions.h"
typedef struct{
	
	int index,active, isDisplaying;
	Image img, close_btn;
}Tuto;


typedef struct{
	Tuto *T;
	Text index_txt;
	int active;
	char indexch[30];   //example at top of page: "1/5"  1= index | 5 = max_index
	int currentIndexDisplaying;
	int currentLastActiveIndex;
	int n;//n=max_index 
	Image next_btn, back_btn;

}Tutorials;

/*TUTORIAL SYSTEM EXPLANATIONS:
All tutorials inside tutorials array are unactive.
If player encounters something or at any point of time a tutorial becomes active => The click on the tutorials button will result in its display
If another tutorial becomes active, the same thing happens. When the player clicks on tutorial button => The first active tutorial is displayed => if other tutorials are displayed, the player can click on next arrow to display the next tutorial (or back) 

*/






void init_tuto(Tuto *t, char filepath_img[], int index);
void init_tutorial(Tutorials *T, int max_index);
void activate_tuto(Tutorials *T);
void display_tuto(Tutorials *T, SDL_Surface *ecran);
void display_tutorials(Tutorials *T, SDL_Surface *ecran);
void handleTutoEvent(Tutorials *T, SDL_Event event);
void liberer_tutorials(Tutorials *T);
#endif /* TUTORIAL_H */

