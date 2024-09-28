#ifndef DOCUMENT_H
#define DOCUMENT_H
#include <SDL/SDL.h>
#include <SDL/SDL_image.h> 
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "fonctions.h"
typedef struct{
	
	int index,active, isDisplaying;
	Image img, close_btn;
	Mix_Chunk *voice;
	int voicePlaying;
}Doc;


typedef struct{
	Doc *T;
	Text index_txt;
	int active;
	char indexch[30];   //example at top of page: "1/5"  1= index | 5 = max_index
	int currentIndexDisplaying;
	int currentLastActiveIndex;
	int n;//n=max_index 
	Image next_btn, back_btn;

}Documents;

/*Document SYSTEM EXPLANATIONS:
Works the same way as tutorial system except, instead of tutorials, the player can read scrolls/ancient text/document etc..

*/






void init_doc(Doc *t, char filepath_img[], int index, char musicpath[]);
void init_documents(Documents *T, int max_index);
void activate_doc(Documents *T);
void display_doc(Documents *T, SDL_Surface *ecran);
void display_documents(Documents *T, SDL_Surface *ecran);
void handleDocEvent(Documents *T, SDL_Event event);
void liberer_documents(Documents *T);
#endif /* DOCUMENT_H */

