#include"enigmetf.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



void InitEnigme(enigmetf *e, char nomfichier[]) {
    FILE *f = NULL;

    e->pos_selected = 0;
    e->rc = 0;
    e->background = IMG_Load("enigme/img/background.png");
    e->button = IMG_Load("enigme/img/button.png");
    e->button_s = IMG_Load("enigme/img/button_s.png");
    
    if (e->background== NULL || e->button_s== NULL || e->button== NULL) {
		printf("Image load error: %s \n", IMG_GetError());
		return;
	}

    //TTF_Init();

    SDL_Color couleur = {255, 255, 255, 255}; // Initialisation explicite de tous les champs de SDL_Color

    e->police = TTF_OpenFont("enigme/Whirlywood-9pRy.ttf", 30);
    if(!e->police) {
    printf("TTF_OpenFont pol: %s | %s\n", TTF_GetError(), SDL_GetError());
}

    e->police1 = TTF_OpenFont("enigme/Whirlywood-9pRy.ttf", 20);
if(!e->police1) {
    printf("TTF_OpenFont pol1: %s | %s\n", TTF_GetError(),SDL_GetError());
}

    srand(time(NULL));
    e->num_question = rand() % 4;

    while (e->num_question == 0) {
        srand(time(NULL));
        e->num_question = rand() % 4;
    }

    char rep1[50];
    char rep2[50];
    char rep3[50];
    strcpy(rep1, "");
    strcpy(rep2, "");
    strcpy(rep3, "");
    int rc = 0; // Réponse correcte

    f = fopen(nomfichier, "r");
    if (f != NULL) {
        int test = 0;
        while (test != e->num_question) {
            fscanf(f, "%[^_]_%[^_]_%[^_]_%[^_]_%d", e->chquestion, rep1, rep2, rep3, &rc);
            test++;
        }
        fclose(f);
    }

    e->rc = rc;

    e->question = TTF_RenderText_Blended(e->police, e->chquestion, couleur);
    e->reponses[0] = TTF_RenderText_Blended(e->police1, rep1, couleur);
    e->reponses[1] = TTF_RenderText_Blended(e->police1, rep2, couleur);
    e->reponses[2] = TTF_RenderText_Blended(e->police1, rep3, couleur);

    e->pos_question.x = 150;
    e->pos_question.y = 300;

    e->pos_reponse1.x = 300;
    e->pos_reponse1.y = 530;

    e->pos_reponse2.x = 300;
    e->pos_reponse2.y = 650;

    e->pos_reponse3.x = 300;
    e->pos_reponse3.y = 770;

    e->pos_reponse1txt.x = e->pos_reponse1.x + 50;
    e->pos_reponse1txt.y = e->pos_reponse1.y + 30;

    e->pos_reponse2txt.x = e->pos_reponse2.x + 50;
    e->pos_reponse2txt.y = e->pos_reponse2.y + 30;

    e->pos_reponse3txt.x = e->pos_reponse3.x + 50;
    e->pos_reponse3txt.y = e->pos_reponse3.y + 30;

    e->image_clock = IMG_Load("enigme/clock1.png");

    e->pos_image_clock.x = 500;
    e->pos_image_clock.y = 100;

    e->single_Clock.w = 57;
    e->single_Clock.h = 81;
    e->single_Clock.x = 0;
    e->single_Clock.y = 0;

    e->clock_num = 0;
}

/*void InitEnigme(enigmetf *e, char nomfichier[])
{

	FILE *f = NULL;

	e->pos_selected = 0;
	e->rc = 0;
	e->background = IMG_Load("img/background.png");
	e->button = IMG_Load("img/button.png");
	e->button_s = IMG_Load("img/button_s.png");

	TTF_Init();
	
	SDL_Color couleur = {255, 255, 255};

	e->police = TTF_OpenFont("Whirlywood-9pRy.ttf", 30);
	e->police1 = TTF_OpenFont("Whirlywood-9pRy.ttf", 20);

	srand(time(NULL));
	e->num_question = rand() % 4;//car le conteur ybda m 0

	while (e->num_question == 0)
	{
		srand(time(NULL));
		e->num_question = rand() % 4;
	}
	

	char rep1[50];
	char rep2[50];
	char rep3[50];
	strcpy(rep1,"");
	strcpy(rep2,"");
	strcpy(rep3,"");
	int rc = 0 ; //rp correcte

	f = fopen(nomfichier, "r");
	if (f != NULL)
	{
		int test = 0;
		while (test != e->num_question)
		{
			fscanf(f, "%[^_]_%[^_]_%[^_]_%[^_]_%d", e->chquestion, rep1, rep2, rep3, &rc);// sauf les caractères spécifiés.
			test++;
		}
		fclose(f);
	}
	srand(time(NULL));
	

	e->rc = rc ;
	


	e->question = TTF_RenderText_Blended(e->police, e->chquestion, couleur);
	e->reponses[0] = TTF_RenderText_Blended(e->police1, rep1, couleur);
	e->reponses[1] = TTF_RenderText_Blended(e->police1, rep2, couleur);
	e->reponses[2] = TTF_RenderText_Blended(e->police1, rep3, couleur);

	e->pos_question.x = 300;
	e->pos_question.y = 300;

	e->pos_reponse1.x = 300;
	e->pos_reponse1.y = 550;

	e->pos_reponse2.x = 300;
	e->pos_reponse2.y = 700;

	e->pos_reponse3.x = 300;
	e->pos_reponse3.y = 850;

	e->pos_reponse1txt.x = e->pos_reponse1.x + 50;
	e->pos_reponse1txt.y = e->pos_reponse1.y + 30;

	e->pos_reponse2txt.x = e->pos_reponse2.x + 50;
	e->pos_reponse2txt.y = e->pos_reponse2.y + 30;

	e->pos_reponse3txt.x = e->pos_reponse3.x + 50;
	e->pos_reponse3txt.y = e->pos_reponse3.y + 30;

	(*e).image_clock = IMG_Load("clock1.png");

	e->pos_image_clock.x = 500;
	e->pos_image_clock.y = 100;

	e->single_Clock.w = 57;
	e->single_Clock.h = 81;
	e->single_Clock.x = 0;
	e->single_Clock.y = 0;

	e->clock_num = 0;

	
}*/

void afficherEnigme(enigmetf e, SDL_Surface *ecran)
{

	SDL_BlitSurface(e.background, NULL, ecran, NULL);
	displayClock(e, ecran);
	SDL_BlitSurface(e.question, NULL, ecran, &e.pos_question);

	switch (e.pos_selected)
	{
	case 0:
		SDL_BlitSurface(e.button, NULL, ecran, &e.pos_reponse1);

		SDL_BlitSurface(e.button, NULL, ecran, &e.pos_reponse2);

		SDL_BlitSurface(e.button, NULL, ecran, &e.pos_reponse3);

		SDL_BlitSurface(e.reponses[0], NULL, ecran, &e.pos_reponse1txt);

		SDL_BlitSurface(e.reponses[1], NULL, ecran, &e.pos_reponse2txt);

		SDL_BlitSurface(e.reponses[2], NULL, ecran, &e.pos_reponse3txt);
		break;

	case 1:
	{
		SDL_BlitSurface(e.button_s, NULL, ecran, &e.pos_reponse1);

		SDL_BlitSurface(e.button, NULL, ecran, &e.pos_reponse2);

		SDL_BlitSurface(e.button, NULL, ecran, &e.pos_reponse3);

		SDL_BlitSurface(e.reponses[0], NULL, ecran, &e.pos_reponse1txt);

		SDL_BlitSurface(e.reponses[1], NULL, ecran, &e.pos_reponse2txt);

		SDL_BlitSurface(e.reponses[2], NULL, ecran, &e.pos_reponse3txt);
	}
	break;
	case 2:
	{
		SDL_BlitSurface(e.button, NULL, ecran, &e.pos_reponse1);

		SDL_BlitSurface(e.button_s, NULL, ecran, &e.pos_reponse2);

		SDL_BlitSurface(e.button, NULL, ecran, &e.pos_reponse3);

		SDL_BlitSurface(e.reponses[0], NULL, ecran, &e.pos_reponse1txt);

		SDL_BlitSurface(e.reponses[1], NULL, ecran, &e.pos_reponse2txt);

		SDL_BlitSurface(e.reponses[2], NULL, ecran, &e.pos_reponse3txt);
	}
	break;
	case 3:
	{
		SDL_BlitSurface(e.button, NULL, ecran, &e.pos_reponse1);

		SDL_BlitSurface(e.button, NULL, ecran, &e.pos_reponse2);

		SDL_BlitSurface(e.button_s, NULL, ecran, &e.pos_reponse3);

		SDL_BlitSurface(e.reponses[0], NULL, ecran, &e.pos_reponse1txt);

		SDL_BlitSurface(e.reponses[1], NULL, ecran, &e.pos_reponse2txt);

		SDL_BlitSurface(e.reponses[2], NULL, ecran, &e.pos_reponse3txt);
	}
	break;
	default:
SDL_Delay (100);
		break;
	}
}

int verify_enigme(enigmetf e, SDL_Surface *ecran)
{
	int v=1;//win

	SDL_Rect pos = {160,330,0,0};


	if (e.pos_selected == e.rc)
	{
		
		SDL_Surface *win;
		win = IMG_Load("enigme/img/youwin.png");//badalha fel init enigme w aamel free 
		SDL_BlitSurface(win, NULL, ecran, &pos);
		
		
	}
	else
	{	v=0;//lost
		SDL_Surface *lost;
		lost = IMG_Load("enigme/img/youlost.png");//badalha fel init enigme w aamel free 
		SDL_BlitSurface(lost, NULL, ecran, &pos);
		
		
	}
//SDL_Delay (100);
	return v;
	
}
void displayClock(enigmetf e, SDL_Surface *ecran){
	SDL_BlitSurface(e.image_clock, &(e.single_Clock), ecran, &e.pos_image_clock);
}
void animer(enigmetf *e)
{

	if (e->clock_num >= 0 && e->clock_num < 8)
	{
		e->single_Clock.x = e->clock_num * e->single_Clock.w;
		e->clock_num++;
	}

	if (e->clock_num == 8)
	{
		e->single_Clock.x = e->clock_num * e->single_Clock.w;
		e->clock_num = 0;
	}
}


void free_Surface_enigme(enigmetf *e)
{

	SDL_FreeSurface(e->background);
	SDL_FreeSurface(e->question);
	SDL_FreeSurface(e->reponses[0]);
	SDL_FreeSurface(e->reponses[1]);
	SDL_FreeSurface(e->reponses[2]);
	SDL_FreeSurface(e->button);
	SDL_FreeSurface(e->button_s);
	SDL_FreeSurface(e->image_clock);
}
