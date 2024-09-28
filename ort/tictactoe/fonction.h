#ifndef FONCTION_H_
#define FONCTION_H_

struct Tic
{
	SDL_Surface *image,*image_x,*image_o,*sur_text;
	int t[9],num_tour,num_j;
	SDL_Rect pos,tabpos[9];
	TTF_Font *font;
	SDL_Color textColor;
	char resultat[20];
};
struct Bord
{
	SDL_Surface *im,*im_lose,*im_win,*im_tie;
	int t[3];
	SDL_Rect pos,tpos[3];
};
struct cellule1
{
	int val;
	struct cellule1 *suiv;
};
typedef struct cellule1 cellule ;
struct cal1
{
	int t[3],nb;
};
typedef struct cal1 cal;
typedef struct Bord bord;
typedef struct Tic tic;
void initialiserTic (tic *t);
void initialiserbord (bord *b);
void afficherbord (bord b,SDL_Surface *ecran);
void afficherTic (tic t,SDL_Surface *ecran);
int atilganer(int tabsuivi[],int *x,int *y,int *z);
int resultat (int t[]);
void liberer( tic t,bord b);
int minimax(int tabsuivi[9], int joueur);
int calcul_coup(int tabsuivi[9]);
void initialiser_cal (cal *c);
int ajouter_cal (cal *c,int i);
#endif
