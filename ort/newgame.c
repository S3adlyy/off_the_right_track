#include <SDL/SDL.h>
#include <SDL/SDL_image.h> 
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "fonctions.h"
#include "newgame.h"
#include "constants.h"

void newgame(SDL_Surface *ecran, int *play, Image menubg_inv_frames[], Image menubg_frames[]) {
	int newgame_active = 1;
	int arrowCount=-1;
	int frames_bg = 5, frames_bg2 =0, frames_button=0;
	int clicked=0, hovered=0,lastHovered=0, i;
	SDL_Rect cur_pos;
	Image selectButtonAnim;
	int unlockedcolor = 255;
	int lockedcolor = 128;
	int level1=0, level2=0, level3=0;
	Text txt_back, txt_lvl3, txt_lvltitle, txt_sas, txt_lvl1, txt_lvl2, txt_lvldesc;
	
	//-open level files and see if they are unlocked or not
	FILE *file = fopen("level1.dat", "rb"); 
    
	if (file != NULL) {
		txt_lvl1 = init_text(SCREEN_W*0.16, 0.40*SCREEN_H, "fonts/emporia-roman.otf", 45, unlockedcolor, unlockedcolor, unlockedcolor); 
		level1=1;
		fclose(file); 
	}
	else{
		txt_lvl1 = init_text(SCREEN_W*0.16, 0.40*SCREEN_H, "fonts/emporia-roman.otf", 45, lockedcolor, lockedcolor, lockedcolor); 
		fclose(file); 
		remove("level1.dat");
	}
	file = fopen("level2.dat", "rb");
	if (file != NULL) {
		txt_lvl2 = init_text(SCREEN_W*0.16, 0.47*SCREEN_H, "fonts/emporia-roman.otf", 45, unlockedcolor, unlockedcolor, unlockedcolor); 
		level2=1;
		fclose(file); 
	    }
	else{
		txt_lvl2 = init_text(SCREEN_W*0.16, 0.47*SCREEN_H, "fonts/emporia-roman.otf", 45, lockedcolor, lockedcolor, lockedcolor); 
		remove("level2.dat");
	}
	file = fopen("level3.dat", "rb"); 
	if (file != NULL) {
		txt_lvl3 = init_text(SCREEN_W*0.16, 0.54*SCREEN_H, "fonts/emporia-roman.otf", 45, unlockedcolor,unlockedcolor , unlockedcolor); 
		level3=1;
		fclose(file); 
	    }
	else{
		txt_lvl3 = init_text(SCREEN_W*0.16, 0.54*SCREEN_H, "fonts/emporia-roman.otf", 45, lockedcolor,lockedcolor , lockedcolor); 
		remove("level3.dat");
	}
	
	//-----
	Image chapter1image, chapter2image, chapter3image;
	init_image(&chapter1image, "newgamelvl1.png",0.6*SCREEN_W, 0.2*SCREEN_H);
	init_image(&chapter2image, "newgamelvl2.png",0.6*SCREEN_W, 0.2*SCREEN_H);
	init_image(&chapter3image, "newgamelvl3.png",0.6*SCREEN_W, 0.2*SCREEN_H);	
	float oldCurPosX=0, oldCurPosY=0;
	SDL_Surface *cursorImage1 = NULL;
	cursorImage1 = init_cursor(&cur_pos);
	init_image(&selectButtonAnim, "game/glowy.png",0,0);
	SDL_Rect buttonAnimFrames[3];
	txt_back = init_text(SCREEN_W*0.86, menuBtnPosY+btnSpacing*4.2, "fonts/emporia-roman.otf", 50, 255, 255, 255); 
	txt_lvltitle = init_text(SCREEN_W*0.6, 0.2*SCREEN_H + 310, "fonts/emporia-roman.otf", 35, 255, 255, 255);  
	txt_lvldesc = init_text(SCREEN_W*0.6, 0.2*SCREEN_H + 350, "fonts/emporia-roman.otf", 35, 255, 255, 255);  
	txt_sas = init_text(SCREEN_W*0.15, 0.27*SCREEN_H, "fonts/emporia-roman.otf", 80, 255, 255, 255); 
	// Render settings page elements
	init_image(&menubg_inv_frames[0], "game/bgmenu_newgame.jpg",0,0);
	
	for(i=0;i<3;i++){
		buttonAnimFrames[i].x=48*i;
		buttonAnimFrames[i].y=0;
		buttonAnimFrames[i].w=48;
		buttonAnimFrames[i].h= 59;
	}
	SDL_Event event;
	Uint32 lastFrameTime = SDL_GetTicks();
	Uint32 lastFrameTime2 = SDL_GetTicks();//start counting frame time
	Uint32 startLoop;
	while(newgame_active){
		startLoop = SDL_GetTicks();
		while (SDL_PollEvent(&event)) {
			switch(event.type){
			case SDL_QUIT: 
			newgame_active = 0;
			*play = 0;
			break;
			
			//Read Keyboard event
			case SDL_KEYDOWN:
				//Si on clique sur le bouton echap     		
				switch(event.key.keysym.sym) {
					
					case SDLK_ESCAPE: //quit
						newgame_active=0;
						*play = 0;
						break;
					case SDLK_f: //fullscreen toggle
						SDL_WM_ToggleFullScreen(ecran);
						break;
					case SDLK_UP:
						if(arrowCount!=0)
							arrowCount--;
						break;
					case SDLK_DOWN:
						arrowCount++;
						break;
						
					case SDLK_RETURN:
						if(hovered==1 && level1){
							*play = 6;
							clicked=1;	
						}
						else if(hovered==2 && level2){
							*play = 7;
							clicked=1;	
						}
						else if(hovered==3 && level3){
							*play = 8;
							clicked=1;	
						}
						else if(hovered==4){
							newgame_active=0;
							*play = 0;	
						}
						break;
					default: break;
				}		
				break;
			
			case SDL_MOUSEBUTTONDOWN:
				if (text_clicked(event, txt_back)) {
				*play = 0;//arjaa lel main menu
				newgame_active=0;
				clicked=0;
				}
				else if(text_clicked(event, txt_lvl1) && level1){
				clicked=1;
				*play=6;//update this number with saved data from bin file(if last played level2 then *play=7)
				}
				else if(text_clicked(event, txt_lvl2)  && level2 || !level2){//check if slot = empty then do ...
				clicked=1;
				*play=7;//update this number with saved data from bin file(if last played level2 then *play=7)
				}
				else if(text_clicked(event, txt_lvl3)  && level3==0 || level3){//check if slot = empty then do ...
				clicked=1;
				*play=8;//update this number with saved data from bin file(if last played level2 then *play=7)
				}
				else
					clicked=0;

				break;
			case SDL_MOUSEMOTION:
				arrowCount=-1;
				updateCursorPosition(&oldCurPosX, &oldCurPosY, &cur_pos, event);
				hovered=0;
				if (text_hovered(event, txt_back)){
					hovered=4;
					}
			    	else if(text_hovered(event, txt_lvl1)){
			    		hovered=1;
			    		}
			    	else if(text_hovered(event, txt_lvl2)){
			    		hovered=2;
			    		}
			    	else if(text_hovered(event, txt_lvl3)){
			    		hovered=3;
			    		}
			    	else
			    		frames_button=0;
			    	break;
		
			}
			
			switch(arrowCount){
				default: 
					arrowCount=-1;
					break;
				case 0:
					hovered=1;break;
				case 1:
					hovered=2;break;
				case 2:
					hovered=3;break;
				case 3:
					hovered=4;break;
				case 4:
					hovered=1;
					arrowCount=0;
					break;
			}
		}
		
		switch(hovered){
			case 0:frames_button=0;break;
			case 1:changeImagePos(&selectButtonAnim, txt_lvl1.pos_txt);break;
			case 2:changeImagePos(&selectButtonAnim, txt_lvl2.pos_txt);break;
			case 3:changeImagePos(&selectButtonAnim, txt_lvl3.pos_txt);break;
			case 4:changeImagePos(&selectButtonAnim, txt_back.pos_txt);break;
			default:break;
		
		}
		if(hovered==0)
			frames_button=0;
		Uint32 currentTime = SDL_GetTicks();
		if (currentTime - lastFrameTime > 50){
	
			if(frames_bg>0 && !clicked){
					frames_bg--;
			    	}
			else if(clicked && frames_bg<5)
				frames_bg++;
		    	
		    	lastFrameTime=currentTime;
		}
		if (currentTime - lastFrameTime2 > 150){		
			if (hovered >= 1 && hovered <= 4)
			{    
				if (frames_button < 2) {
				    frames_button++;
				}
		
	    		}

		    	lastFrameTime2=currentTime;
		}

	//	printf("\nframes bg: %d", frames_bg);
		afficher_image(menubg_inv_frames[frames_bg], ecran);//frames_bg=0
		if (hovered >= 1 && hovered <= 4)
			afficher_sprite(selectButtonAnim, ecran, &buttonAnimFrames[frames_button]);
		afficher_text(ecran, &txt_back, "Back");
		switch(hovered){
			case 1: afficher_text(ecran, &txt_lvltitle, "Chapter 1: Right to Non-Discrimination");
				afficher_text(ecran, &txt_lvldesc, "Ancient Egypt Era, 330 B.C");
				afficher_image(chapter1image, ecran);
				break;
			case 2: afficher_text(ecran, &txt_lvltitle, "Chapter 2: Right to Education");
				afficher_text(ecran, &txt_lvldesc, "Italy Renaissance Era, 1443");
				afficher_image(chapter2image, ecran);
				break;
			case 3: afficher_text(ecran, &txt_lvltitle, "Chapter 3: Right to Gender Equality");
				afficher_text(ecran, &txt_lvldesc, "Britain Industrial Revolution, 1812");
				afficher_image(chapter3image, ecran);
				break;
		
		}
		
		afficher_text(ecran, &txt_sas, "Select a Level");
		afficher_text(ecran, &txt_lvl1, "Chapter 1");
		afficher_text(ecran, &txt_lvl2, "Chapter 2");
		afficher_text(ecran, &txt_lvl3, "Chapter 3");
		//SDL_FillRect(ecran, &(SDL_Rect){0.6*SCREEN_W, 0.2*SCREEN_H, 500, 300}, SDL_MapRGB(ecran->format, 255, 255, 255));
		displayCursor(cursorImage1,cur_pos, oldCurPosX,oldCurPosY,ecran);
		if(*play>=6 && frames_bg==5)
			newgame_active = 0;
			
		if(currentTime - startLoop <1000/FRAMES_PER_SECOND)
			SDL_Delay((1000/FRAMES_PER_SECOND) - (currentTime-startLoop));
		SDL_Flip(ecran);

	}
	liberer_image(selectButtonAnim);
	liberer_text(&txt_back);
	liberer_text(&txt_sas);
	liberer_text(&txt_lvltitle);
	liberer_text(&txt_lvl1);
	liberer_text(&txt_lvl2);
	liberer_text(&txt_lvl3);
	liberer_text(&txt_lvldesc);
	liberer_image(chapter1image);
	liberer_image(chapter2image);
	liberer_image(chapter3image);
	if (cursorImage1 != NULL) {
	    SDL_FreeSurface(cursorImage1);
	    cursorImage1=NULL;
	} else {
	    fprintf(stderr, "Error: cursorImage1 is NULL\n");
	}
}


