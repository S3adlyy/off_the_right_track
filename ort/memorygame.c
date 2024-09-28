#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "fonctions.h"
#include "constants.h"
#include "memorygame.h"
#include <string.h>

//ADD TIMER -> IF TIMER ENDS -> PLAYER ISNT REWARDED
//ADD FIRST SECONDS ANIMATION TO SHOW THE CARDS TO THE PLAYER


void init_memorygame_spritesheet(SDL_Rect T[]){
	int i;
	for(i=0;i<6;i++){
		T[i].x=253*i;
		T[i].y=0;
		T[i].w=253;
		T[i].h= 444;
	}
	T[0].h = 400;//for clicking behaviour
	T[5].h = 400;
}

void init_memorygame(Image T[]){
	int x=0, i;
	for(i=0;i<6;i++){
	if(x==3)
		x=0;//go back to line and place cards
	if(i<3)
		init_image(&T[i], "game/memorygamespritesheetraw.png",SCREEN_W*0.17+480*x,SCREEN_H*0.13);
	else
		init_image(&T[i], "game/memorygamespritesheetraw.png",SCREEN_W*0.17+480*x,SCREEN_H*0.52);
	x++;
	size_image(&T[i], 253, 400);//for clicking behaviour
	}

}

void afficher_memorygame(SDL_Rect T[], Image T2[], SDL_Surface *ecran, int cardsframes[]){
	int i;
	for(i=0;i<6;i++){
		afficher_sprite(T2[i], ecran, &T[cardsframes[i]]);
	}	

}

void init_memorygame_card_frames(int cardsframes[]){
	int i;
	for(i=0;i<6;i++){
		cardsframes[i]=0;
	}	
}

void init_memorygame_answers(int memorygame_answers[])//testing
{

	int x=1;
	//fill the array with numbers 1, 2, 3
	for (int i = 0; i < 6; i++) {
		memorygame_answers[i] = x;
		x++;
		if(i==2)
			x=1;
	}

	// shuffle 
	for (int i = 0; i < 6; i++) {
		int temp = memorygame_answers[i];
		int rand = random_int(0, 5);
		memorygame_answers[i] = memorygame_answers[rand];
		memorygame_answers[rand] = temp;
	}
	
	//print to check
	printf("---------------------------------------------------------------------\n");
	for (int i = 0; i < 6; i++) {
		printf("%d | ", memorygame_answers[i]);
		if(i==2)
			printf("\n");
	}
	
	
	

}
int check_memorygame(int cardsframes[], int memorygame_answers[], int selectedcard1, int selectedcard2){//return 1 (else 0) and add 2 to the score, if score reaches 6 (the player guessed all cards, then the player wins) 

//if two cards selected are right, keep their frame on last frame 
//else, the cards should be flipped back to frame 0
	if(memorygame_answers[selectedcard1]==memorygame_answers[selectedcard2])
		return 1;
	return 0;//selected cards frames should drop


}


int rewardPlayer(SDL_Surface *ecran, SDL_Surface *cursorImage4,SDL_Rect cur_pos, float oldCurPosX, float oldCurPosY, int *r){
	Image life, time, influence, bg;
	char timerewardtxt[13], resrewardtxt[14], hprewardtxt[14];
	init_image(&bg, "game/rewardbg.jpg",0,0);
	init_image(&life, "game/potion.png",SCREEN_W*0.04,SCREEN_H*0.3);
	init_image(&time, "game/back-in-time.png",SCREEN_W*0.36,SCREEN_H*0.1);
	init_image(&influence, "game/influence_pic.png",SCREEN_W*0.67,SCREEN_H*0.3);
	Text txt_text= init_text(SCREEN_W*0.32, SCREEN_H*0.01, "fonts/emporia-roman.otf", 80, 255, 255, 255); 
	Text txt_addlife= init_text(SCREEN_W*0.13, SCREEN_H*0.8, "fonts/emporia-roman.otf", 50, 255, 255, 255); 
	Text txt_addtime= init_text(SCREEN_W*0.43, SCREEN_H*0.6, "fonts/emporia-roman.otf", 50, 255, 255, 255); 
	Text txt_addresources= init_text(SCREEN_W*0.74, SCREEN_H*0.8, "fonts/emporia-roman.otf", 50, 255, 255, 255);
	int rand_time = random_int(10, 30); 
	int rand_resource = random_int(5, 14);
	int rand_hp = random_int(1, 3);
	sprintf(timerewardtxt, "+%d Seconds",rand_time);
	sprintf(resrewardtxt, "+%d Influence",rand_resource);
	switch(rand_hp){
		case 1: sprintf(hprewardtxt, "+25 HP");break;
		case 2: sprintf(hprewardtxt, "+50 HP");break;
		case 3: sprintf(hprewardtxt, "+75 HP");break;
		default: sprintf(hprewardtxt, "+25 HP");
	
	}
	SDL_Event event;
	int quit=1;
	while(quit==1){
		//SDL_FillRect(ecran, &(SDL_Rect){0, 0, 1920, 1080}, SDL_MapRGB(ecran->format, 90, 90, 90));
		afficher_image(bg, ecran);
		afficher_text(ecran, &txt_text, "Choose a reward carefully.");
		afficher_text(ecran, &txt_addlife, hprewardtxt);
		afficher_text(ecran, &txt_addtime, timerewardtxt);
		afficher_text(ecran, &txt_addresources, resrewardtxt);
		afficher_image(life, ecran);
		afficher_image(time, ecran);
		afficher_image(influence, ecran);
	
		while(SDL_PollEvent(&event)){
			switch(event.type){
			
				case SDL_MOUSEBUTTONDOWN:
						if (text_clicked(event, txt_addlife) || button_clicked(event, life)) {
							*r = rand_hp;							
							return 1;
							}
						else if(text_clicked(event, txt_addtime) || button_clicked(event, time)){
							*r = rand_time;
							return 2;
							}
						else if(text_clicked(event, txt_addresources )|| button_clicked(event, influence)){
							*r = rand_resource;
							return 3;
							}
						quit=0;
					    break;
				case SDL_MOUSEMOTION:
					updateCursorPosition(&oldCurPosX, &oldCurPosY, &cur_pos, event);
					break;
			
			}
		
		
		}
		displayCursor(cursorImage4,cur_pos, oldCurPosX,oldCurPosY,ecran);
		SDL_Flip(ecran);
	}
	liberer_image(life);
	liberer_image(bg);
	liberer_image(time);
	liberer_image(influence);
	liberer_text(&txt_addlife);
	liberer_text(&txt_addtime);
	liberer_text(&txt_addresources);
	
}
//if player won, it displays a page where the player gets to choose between 3 rewards(time/hp/resources)


//LEVEL 2 REWARD --------------------------
int rewardPlayer2(SDL_Surface *ecran, SDL_Surface *cursorImage4,SDL_Rect cur_pos, float oldCurPosX, float oldCurPosY, int *r){
	Image life, time, influence, bg;
	char timerewardtxt[13], resrewardtxt[14], hprewardtxt[14];
	init_image(&bg, "game/rewardbg.jpg",0,0);
	init_image(&life, "game/potion.png",SCREEN_W*0.04,SCREEN_H*0.3);
	init_image(&time, "game/back-in-time.png",SCREEN_W*0.36,SCREEN_H*0.1);

	Text txt_text= init_text(SCREEN_W*0.32, SCREEN_H*0.01, "fonts/emporia-roman.otf", 80, 255, 255, 255); 
	Text txt_addlife= init_text(SCREEN_W*0.13, SCREEN_H*0.8, "fonts/emporia-roman.otf", 50, 255, 255, 255); 
	Text txt_addtime= init_text(SCREEN_W*0.43, SCREEN_H*0.6, "fonts/emporia-roman.otf", 50, 255, 255, 255); 
	Text txt_addresources= init_text(SCREEN_W*0.74, SCREEN_H*0.8, "fonts/emporia-roman.otf", 50, 255, 255, 255);
	int rand_time = random_int(10, 30); 
	int rand_resource = random_int(5, 14);
	int rand_hp = random_int(1, 3);
	sprintf(timerewardtxt, "+%d Seconds",rand_time);
	if(random_int(1, 2)==2){
		sprintf(resrewardtxt, "+%d Bags",rand_resource);
		init_image(&influence, "game/back-in-time.png",SCREEN_W*0.67,SCREEN_H*0.3);  //CHANGE TO BAG PIC
		}
	else{
		sprintf(resrewardtxt, "+%d Pens ",rand_resource);
		init_image(&influence, "game/back-in-time.png",SCREEN_W*0.67,SCREEN_H*0.3);  //CHANGE TO PEN PIC
	}
	switch(rand_hp){
		case 1: sprintf(hprewardtxt, "+25 HP");break;
		case 2: sprintf(hprewardtxt, "+50 HP");break;
		case 3: sprintf(hprewardtxt, "+75 HP");break;
		default: sprintf(hprewardtxt, "+25 HP");
	
	}
	SDL_Event event;
	int quit=1;
	while(quit==1){
		//SDL_FillRect(ecran, &(SDL_Rect){0, 0, 1920, 1080}, SDL_MapRGB(ecran->format, 90, 90, 90));
		afficher_image(bg, ecran);
		afficher_text(ecran, &txt_text, "Choose a reward carefully.");
		afficher_text(ecran, &txt_addlife, hprewardtxt);
		afficher_text(ecran, &txt_addtime, timerewardtxt);
		afficher_text(ecran, &txt_addresources, resrewardtxt);
		afficher_image(life, ecran);
		afficher_image(time, ecran);
		afficher_image(influence, ecran);
	
		while(SDL_PollEvent(&event)){
			switch(event.type){
			
				case SDL_MOUSEBUTTONDOWN:
						if (text_clicked(event, txt_addlife) || button_clicked(event, life)) {
							*r = rand_hp;							
							return 1;
							}
						else if(text_clicked(event, txt_addtime) || button_clicked(event, time)){
							*r = rand_time;
							return 2;
							}
						else if(text_clicked(event, txt_addresources )|| button_clicked(event, influence)){
							*r = rand_resource;
							return 3;
							}
						quit=0;
					    break;
				case SDL_MOUSEMOTION:
					updateCursorPosition(&oldCurPosX, &oldCurPosY, &cur_pos, event);
					break;
			
			}
		
		
		}
		displayCursor(cursorImage4,cur_pos, oldCurPosX,oldCurPosY,ecran);
		SDL_Flip(ecran);
	}
	liberer_image(life);
	liberer_image(bg);
	liberer_image(time);
	liberer_image(influence);
	liberer_text(&txt_addlife);
	liberer_text(&txt_addtime);
	liberer_text(&txt_addresources);
	
}
















