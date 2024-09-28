#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "enigmetf.h"
#include "enigme.h"

int enigme(SDL_Surface *ecran){
    SDL_Event event;
    enigmetf eg;
    int quitter=1;
    int gamemode = 0; // (0 pour l'énigme, 1 pour le résultat)
    int i = 0;
    InitEnigme(&eg, "enigme/fichier.txt");

    while (quitter) {
        if (gamemode == 0) {
            srand(time(NULL)); // générateur aléatoire
            afficherEnigme(eg, ecran);
            animer(&eg);

            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_KEYDOWN:
                        switch (event.key.keysym.sym) {
                            case SDLK_DOWN:
                            	i++;
				if(i>=4){i=1;}
				if(i==1){eg.pos_selected = 1;}
				if(i==2){eg.pos_selected = 2;}
				if(i==3){eg.pos_selected = 3;}
                                break;
                            case SDLK_UP:
                                i--;
				if(i<=0){i=3;}
				if(i==1){eg.pos_selected = 1;}
				if(i==2){eg.pos_selected = 2;}
				if(i==3){eg.pos_selected = 3;}
                                break;
                            case SDLK_RETURN:
                                gamemode = verify_enigme(eg, ecran);
                                SDL_Delay(2000);
                                break;
                            case SDLK_ESCAPE:
                                quitter = 0;
                                break;
                            default:
                                break; // Cas par défaut pour les touches non traitées
                        }
                        break;
                    case SDL_MOUSEMOTION:
                        if (event.motion.x > eg.pos_reponse1.x && event.motion.x < eg.pos_reponse1.x + 240 &&
                            event.motion.y > eg.pos_reponse1.y && event.motion.y < eg.pos_reponse1.y + 90)
                            eg.pos_selected = 1;
                        else if (event.motion.x > eg.pos_reponse2.x && event.motion.x < eg.pos_reponse2.x + 240 &&
                                 event.motion.y > eg.pos_reponse2.y && event.motion.y < eg.pos_reponse2.y + 90)
                            eg.pos_selected = 2;
                        else if (event.motion.x > eg.pos_reponse3.x && event.motion.x < eg.pos_reponse3.x + 240 &&
                                 event.motion.y > eg.pos_reponse3.y && event.motion.y < eg.pos_reponse3.y + 90)
                            eg.pos_selected = 3;
                        else
                            eg.pos_selected = 0;
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        if (event.button.button == SDL_BUTTON_LEFT) {
                            if (eg.pos_selected == 1) {
                                gamemode = verify_enigme(eg, ecran);
                                SDL_Flip(ecran);
                                SDL_Delay(2000);
                                quitter=0;
				return gamemode;
                            
                            }
                            else if(eg.pos_selected == 2 )
				{
					gamemode=verify_enigme(eg,ecran);
					SDL_Flip(ecran);
					SDL_Delay(2000);	
					return gamemode;
				}
			     else if(eg.pos_selected == 3 )
				{
				gamemode=verify_enigme(eg,ecran);
				SDL_Flip(ecran);
				SDL_Delay(2000);
				quitter=0;
				return gamemode;
				}
                        }
                        
                        break;
                    default:
                        break; // Cas par défaut pour les événements non traités
                }
            }
        }

        SDL_Flip(ecran);
        SDL_Delay(25);
    }

    free_Surface_enigme(&eg);
    //TTF_CloseFont(eg.police1);
    TTF_CloseFont(eg.police);
}
