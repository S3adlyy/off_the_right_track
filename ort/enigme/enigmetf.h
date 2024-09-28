#ifndef enigmetf_h
#define enigmetf_h
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#define SCREEN_W 1920
#define SCREEN_H 1080


typedef struct{
	SDL_Rect pos;
	SDL_Surface *img;
}image;

typedef struct {
	SDL_Rect position;
	TTF_Font *font;
	SDL_Surface * surfaceTexte;
	SDL_Color textColor;
	char texte [50];
}text;

typedef struct
{ // type define structure

	SDL_Surface *background;  // l background ali besh nafficher fiha l question o les reponses
	SDL_Surface *question;	  // l surface ali besh nafficher feha l question
	SDL_Surface *reponses[4]; // l surface ali besh nafficher feha les reponses
	SDL_Surface *button;	  // l button ali besh nenzl alih
	SDL_Surface *button_s;

	char  chquestion[50];
	char chrep[2][50];
	int rc; // l reponse l s7i7a yam loula wala l thenya wala l theltha


	int pos_selected;	   // 1 2 3
	SDL_Rect pos_question; // l position mta3 l question
	SDL_Rect pos_reponse1; // l position mta3 lreponse
	SDL_Rect pos_reponse2; // l position mta3 l reponse
	SDL_Rect pos_reponse3; // l position mta3 l reponse
	int num_question;

	SDL_Rect pos_reponse1txt; // l position mta3 l txt ali besh nhoto fouk l image ta3 l reponse
	SDL_Rect pos_reponse2txt; // l position mta3 l txt ali besh nhoto fouk l image ta3 l reponse
	SDL_Rect pos_reponse3txt; // l position mta3 l txt ali besh nhoto fouk l image ta3 l reponse		  
	TTF_Font *police;
	TTF_Font *police1;

	SDL_Surface *image_clock;
	SDL_Rect pos_image_clock;

	SDL_Rect single_Clock;

	int clock_num;

}enigmetf;





void InitEnigme(enigmetf *e, char nomfichier[]);
void afficherEnigme(enigmetf e, SDL_Surface *ecran);
void displayClock(enigmetf e, SDL_Surface *ecran);
int verify_enigme(enigmetf e, SDL_Surface *ecran);
void animer(enigmetf *e);
void free_Surface_enigme(enigmetf *e);
#endif
