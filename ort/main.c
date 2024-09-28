#include <SDL/SDL.h>//inclure SDL.h  pour permettre au programme d'être portable sur tous les systèmes
#include <SDL/SDL_image.h> //Pour manipuler des images ayants des types autre que b
#include <SDL/SDL_mixer.h>//Pour manipuler de l’audio.
#include <SDL/SDL_ttf.h>
#include "fonctions.h"
#include "settings.h"
#include "newgame.h"
#include <pthread.h>
#include "constants.h"


/**
	* @file main.c
	*@brief Testing program
	*@author Artemis Team
	*@version 0.1
	*@date May 02 2024
*/

//GLOBAL VARS
const float STEP_WEIGHTS[5] = {0.2,0.05,0.15, 0.24, 0.36};



// Initialize loading progress
float totalProgress;
Image loadingScreen;
Image menubg, btn_newgame, btn_credits,btn_settings, btn_howtoplay, btn_exit, menu_logo, credits_parchment, htp_parchment, exit_parchment; 
Image menubg_frames[6];
Image menubg_inv_frames[6], menulogo_frames[6], menu_animation_frames[6];
SDL_Rect btn_exit_frames[6], btn_newgame_frames[4],btn_credits_frames[4],btn_settings_frames[4],btn_howtoplay_frames[4];
SDL_Rect positiontext, cur_pos;
Mix_Music *menutheme=NULL;
Mix_Chunk *button_click_chunk=NULL;
int volumeMusic, volumeSfx, fullscreen=2, logo_frames=0, reachedLast=0;
int i;
Text txt_back1, txt_yes, txt_no;


// Load resources in separate thread
void* loadResources(void* arg) {
	//Load images
	init_image(&menubg_inv_frames[0], "game/bgmenuf1.jpg",0,0);
	init_image(&menubg_inv_frames[1], "game/bgmenu_inverted_f2.jpg",0,0);
	init_image(&menubg_inv_frames[2], "game/bgmenu_inverted_f3.jpg",0,0);
	init_image(&menubg_inv_frames[3], "game/bgmenu_inverted_f4.jpg",0,0);
	init_image(&menubg_inv_frames[4], "game/bgmenu_inverted_f5.jpg",0,0);
	init_image(&menubg_inv_frames[5], "game/bgmenu_inverted_f6.jpg",0,0);
	loadSettings(&volumeMusic, &volumeSfx);
	
	
	init_image(&menubg_frames[0], "game/bgmenuf1.jpg",0,0);
	init_image(&menubg_frames[1], "game/bgmenu_frame2.jpg",0,0);
	init_image(&menubg_frames[2], "game/bgmenu_frame3.jpg",0,0);
	init_image(&menubg_frames[3], "game/bgmenu_frame4.jpg",0,0);
	init_image(&menubg_frames[4], "game/bgmenu_frame5.jpg",0,0);
	init_image(&menubg_frames[5], "game/bgmenu_frame6.jpg",0,0);
	
	//init menu animation frames
	init_image(&menu_animation_frames[0], "game/menuanim/F1.png", 0, 0);
	init_image(&menu_animation_frames[1], "game/menuanim/F2.png", 0, 0);
	init_image(&menu_animation_frames[2], "game/menuanim/F3.png", 0, 0);
	init_image(&menu_animation_frames[3], "game/menuanim/F4.png", 0, 0);
	init_image(&menu_animation_frames[4], "game/menuanim/F5.png", 0, 0);
	init_image(&menu_animation_frames[5], "game/menuanim/F6.png", 0, 0);
	
	//init logo frames
	init_image(&menulogo_frames[0], "game/F1.png",SCREEN_W*0.13,SCREEN_H*0.135);
	init_image(&menulogo_frames[1], "game/F2.png",SCREEN_W*0.13,SCREEN_H*0.135);
	init_image(&menulogo_frames[2], "game/F3.png",SCREEN_W*0.13,SCREEN_H*0.135);
	init_image(&menulogo_frames[3], "game/F4.png",SCREEN_W*0.13,SCREEN_H*0.135);
	init_image(&menulogo_frames[4], "game/F5.png",SCREEN_W*0.13,SCREEN_H*0.135);
	init_image(&menulogo_frames[5], "game/F6.png",SCREEN_W*0.13,SCREEN_H*0.135);
	
	
	//init rest of menu elements
	//init_image(&menu_logo, "game/menulogo.png", SCREEN_W*0.13, SCREEN_H*0.135);
	init_image(&btn_newgame, "game/newgame_sheet.png", menuBtnPosX, menuBtnPosY);
	init_image(&btn_credits, "game/credits_sheet.png", menuBtnPosX,  menuBtnPosY+btnSpacing);
	init_image(&btn_settings, "game/settings_sheet.png", menuBtnPosX,  menuBtnPosY+btnSpacing*2);
	init_image(&btn_howtoplay, "game/howtoplay_sheet.png", menuBtnPosX,  menuBtnPosY+btnSpacing*3);
	init_image(&btn_exit,"game/72exit_sprite_sheet.png" , SCREEN_W*0.9, menuBtnPosY+btnSpacing*4.4);
	init_image(&credits_parchment, "game/Credits.png", SCREEN_W*0.6, SCREEN_H*0.1);
	init_image(&htp_parchment, "game/howtoplay.png", SCREEN_W*0.1, SCREEN_H*0.1);
	init_image(&exit_parchment, "game/exit_parchment.png", SCREEN_W*0.3, SCREEN_H*0.3);
	//set image size for spritesheets
	size_image(&btn_newgame, 155, 72);//155
	size_image(&btn_credits, 106, 72);//106
	size_image(&btn_settings, 110, 72);
	size_image(&btn_howtoplay, 162, 72);
	size_image(&btn_exit, 70, 72);
	
	totalProgress += STEP_WEIGHTS[0];
	SDL_Delay(500); 

	//exit clips
	
	for(i=0;i<6;i++){
		if(i<4){
		btn_exit_frames[i].x = 80*i;
		btn_exit_frames[i].w = 80;
		}
		else if(i==4){
			btn_exit_frames[i].x = 318;
			btn_exit_frames[i].w = 59;
		}
		else{
			btn_exit_frames[i].x = 377;
			btn_exit_frames[i].w = 68;
		}
		btn_exit_frames[i].y = 0;
		btn_exit_frames[i].h = 72;
	}
	totalProgress += STEP_WEIGHTS[1];
	SDL_Delay(150); 
	//clip menu button spritesheets
	for(i=0;i<4;i++){
		//clip new game sprites row
		btn_newgame_frames[i].x=181*i;
		btn_newgame_frames[i].y=0;
		btn_newgame_frames[i].w=181;
		btn_newgame_frames[i].h= 72;
		//clip credits sprites row
		btn_credits_frames[i].x=181*i;
		btn_credits_frames[i].y=0;
		btn_credits_frames[i].w=181;
		btn_credits_frames[i].h= 72;
		//clip settings sprites row
		btn_settings_frames[i].x=181*i;
		btn_settings_frames[i].y=0;
		btn_settings_frames[i].w=181;
		btn_settings_frames[i].h= 72;
		//clip how to play sprites row
		btn_howtoplay_frames[i].x=181*i;
		btn_howtoplay_frames[i].y=0;
		btn_howtoplay_frames[i].w=181;
		btn_howtoplay_frames[i].h= 72;
	
	}
	
	
	// Update progress
	totalProgress += STEP_WEIGHTS[2];
	SDL_Delay(200); 

	//Load sounds
	if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024)==-1)//STEREO AUDIO OR MONO(ADD TO SETTINGS) | ONLY OPEN AUDIO ONCE
	{
		printf("Error Opening Audio[music|chunk]: %s \n", Mix_GetError());
		Mix_CloseAudio();
		return NULL;
	}  
        button_click_chunk = init_chunk("game/onclick_begingame.wav");
        if(button_click_chunk==NULL)
        	printf("%s \n %s \n", SDL_GetError(), Mix_GetError());
        init_music(menutheme, "game/menu_theme.wav");
        Mix_VolumeChunk(button_click_chunk, volumeSfx);
	Mix_VolumeMusic(volumeMusic);
	// Update progress
	totalProgress += STEP_WEIGHTS[3];
	SDL_Delay(500); 
	
	//Load fonts
	
	txt_back1 = init_text(SCREEN_W*0.88, menuBtnPosY+btnSpacing*4.5, "fonts/emporia-roman.otf", 50, 255, 255, 255); 
	txt_yes = init_text(exit_parchment.pos1.x+0.35*exit_parchment.img->w, exit_parchment.pos1.y+0.6*exit_parchment.img->h, "fonts/emporia-roman.otf", 50, 0, 0, 0);
	txt_no = init_text(exit_parchment.pos1.x+0.65*exit_parchment.img->w, exit_parchment.pos1.y+0.6*exit_parchment.img->h, "fonts/emporia-roman.otf", 50, 0, 0, 0);
	//change to brown 
	// Update progress
	totalProgress += STEP_WEIGHTS[4];
	SDL_Delay(500); 
	totalProgress += 0.01;
	return NULL;
}


void menu(SDL_Surface *ecran, int *page)
{
	printf("entered menu\n");
	//menu element (buttons, logos, etc..)
	SDL_Event event, event1;//event
	int quitter = 1, clickedYes, clickedNo, i; //condition arret boucle de jeu
	int hovered=0; //0-not hovered 1- new game button 2- credits button 3- setting button 4- how to play btn 5- exit button
	int play=0;//Déclaration d'un compteur pour la mise à jour aprés un événement(clic sur bouton)
	int arrowCount=-1;
	//music 
	float oldCurPosX=0, oldCurPosY=0;
	int frame=0;
	int frame_newgame = 0, frame_credits = 0, frame_settings = 0, frame_howtoplay = 0, frame_exit = 0, frames_bg=0, frames_menuanim=0, k=-1;//k is used for frame_menuanim
	int frames[5] = {0};
	totalProgress=0;
	//load cursor image
	SDL_Surface *cursorImage = init_cursor(&cur_pos);
	
	//fin init
	//hide cursor
	HideCursor();
	///////////////////////////////INITIALISATION ASSETS
	
	//init bg
	SDL_Rect progressBarRect;
	char progressText[20];
	Text loadingTxt = init_text(SCREEN_W / 2, SCREEN_H / 2+40, "fonts/emporia-roman.otf", 70, 255, 255, 255); ;
	init_image(&loadingScreen, "game/bgmenu_newgame.jpg",0,0);
	
      	 // Create loading screen thread
	pthread_t thread;
	pthread_create(&thread, NULL, loadResources, NULL);
	printf("thread created \n total progress: %.2f", totalProgress-1);

	// thread loading resources loop
	while (totalProgress <= 1.0) {
		// Calculate the progress bar rectangle
		progressBarRect.x = SCREEN_W / 4;
		progressBarRect.y = SCREEN_H / 2;
		progressBarRect.w = totalProgress * SCREEN_W / 2;
		progressBarRect.h = 30;

		

		// Render the loading screen image
		afficher_image(loadingScreen, ecran);

		// Render the progress bar
		SDL_FillRect(ecran, &progressBarRect, SDL_MapRGB(ecran->format, 255, 255, 255));

		// Update the loading text with the progress percentage
		snprintf(progressText, 20, "Loading... %.0f%%", totalProgress * 100);//converti float l text str
		afficher_text(ecran, &loadingTxt, progressText);

		// Update the screen
		SDL_Flip(ecran);

		// Update progress in the loading thread
		SDL_Delay(10); 
	}


	// Wait for loading thread to finish
	pthread_join(thread, NULL);
	
	liberer_image(loadingScreen);
	liberer_text(&loadingTxt);
      	
      	//animation frame time init
	Uint32 lastFrameTime=SDL_GetTicks();
	Uint32 lastFrameTime2 = SDL_GetTicks();//start counting frame time
	Uint32 lastFrameTime3 = SDL_GetTicks();//start counting frame time
	Uint32 lastFrameTime4 = SDL_GetTicks();//start counting frame time
	Uint32 animationInterval = 100;//animation time: 0.
	Uint32 startLoop;
	
	while(quitter) 
	{
	startLoop = SDL_GetTicks();
	//Lecture de l'événement	
	while(SDL_PollEvent(&event)){
		switch(event.type)
		 { 
			// exit if the window is closed
			case SDL_QUIT: quitter = 0; *page=0;
			break;
			
			//Read Keyboard event
			case SDL_KEYDOWN:
				//Si on clique sur le bouton echap     		
				switch(event.key.keysym.sym) {
					
					case SDLK_ESCAPE: //quit
						if(play==0){
						play=5;
						}
						else
							play=0;
						break;
					case SDLK_f: //fullscreen toggle
						SDL_WM_ToggleFullScreen(ecran);
						if(fullscreen ==1)
							fullscreen=2;
						else
							fullscreen=1;
						break;
					case SDLK_s:
						play=3;break;
					case SDLK_h:
						play=4;break;
					case SDLK_n:
						play=1;break;
					case SDLK_c:
						play=2;break;
					case SDLK_UP:
						if(play != 5 && arrowCount!=0)
							arrowCount--;
						break;
					case SDLK_DOWN:
						if(play!=5)
							arrowCount++;
						break;
						
					case SDLK_RETURN:
						play=hovered;
						if(Mix_Playing(2)==0)
							Mix_PlayChannel(2,button_click_chunk, 0);
							
						break;
					default: break;
				}		
				break;
			//Read Mouse Button Down Events
		 	case SDL_MOUSEBUTTONDOWN:
	 			//exit click check yes or no
	 			if(play==5){
	 				if(text_clicked(event, txt_yes)) {
	    					    	quitter = 0;
	    						*page = 0;
			      				printf("clicked yes\n");
						} else if(text_clicked(event, txt_no)) {
						    	clickedNo = 1;
						    	play = 0;
	    					    	printf("clicked no\n");
						}
	 			}
	 			
	 			//check other buttons clicks
		 		if(event.button.state == SDL_PRESSED){
		 		
		 		
					if(button_clicked(event, btn_exit))
					{
						if(play==0 || play==2){
						if(play!=5)
							Mix_PlayChannel(2,button_click_chunk, 0);
						play=5;
						}
						//SDL_Delay(2000);
					}
					else if(button_clicked(event, btn_newgame))
					{
						if(play==0 || play==2){
						if(play!=1)
							Mix_PlayChannel(2,button_click_chunk, 0);
						play=1;
						}
					}
					else if(button_clicked(event, btn_credits))
					{
						if(play==0){
						if(play!=2)//fix button credits playing sound fx multiple times if hovered on it multiple times
							Mix_PlayChannel(2,button_click_chunk, 0);
						play=2;}
					}
					else if(button_clicked(event, btn_settings))
					{
						if(play==0 || play==2 ){
						if(play!=3)
							Mix_PlayChannel(2,button_click_chunk, 0);
						play=3;}
					}
					else if(button_clicked(event, btn_howtoplay))
					{	if(play==0 || play==2){
						if(play!=4)
							Mix_PlayChannel(2,button_click_chunk, 0);
						play=4;}
					}
					else
					{	if(play==0 ){
						Mix_HaltChannel(2);//channel 1 is for button click chunk
						play=0;}
					}
		        	}
		        	break;
		        //read mouse movement
		 	case SDL_MOUSEMOTION:
		 		arrowCount=-1;
		 		updateCursorPosition(&oldCurPosX, &oldCurPosY, &cur_pos, event);
				hovered=0;
				//if exit parchment is dislayed, then you can't hover on other buttons
				if(play!=5){
				if(button_hovered(event, btn_newgame))
					hovered=1;
				else if(button_hovered(event, btn_credits))
					hovered=2;
				else if(button_hovered(event, btn_settings))
					hovered=3;
				else if(button_hovered(event, btn_howtoplay))
					hovered=4;
				else if(button_hovered(event, btn_exit))
					hovered=5;
				
				}
				break;
			default: break;
		 	
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
		hovered=5;break;
	case 5:
		hovered=1;
		arrowCount=0;
		break;
	}
	
	}

	//If clicked on something, do this
	//arrow selection
	
	//buttons animation
    	
	// Reset frames for buttons not hovered
	for (i = 0; i < 5; i++) {
	    if (hovered != i + 1) {
		frames[i] = 0;
	    }
	}
	
	Uint32 currentTime = SDL_GetTicks();//get current frame time to substract it from last frame time

	// Update animation frames only if hovered
	if (currentTime - lastFrameTime > animationInterval) {
		if (hovered >= 1 && hovered <= 5)
		{	     
			if (frames[hovered - 1] < (hovered == 5 ? 5 : 3)) {
			    (frames[hovered - 1])++;
			}
	    	}
	    	
	    lastFrameTime=currentTime;//update the time the animation happened
	}
	
	if (currentTime - lastFrameTime2 > 50){
	
		if(play==1){
	    		if(frames_bg<5)
				frames_bg++;
		    	}
		else{
			if(frames_bg>0)
				frames_bg--;
	    	}
	    	lastFrameTime2=currentTime;
	}
	
	if (currentTime - lastFrameTime3 > 150){
			if(logo_frames==5)
				reachedLast=1;
			if(logo_frames==0)
				reachedLast=0;
			if(reachedLast==1)
				logo_frames--;
			else
				logo_frames++;
				
	    	lastFrameTime3=currentTime;
	}
	if (currentTime - lastFrameTime4 > 270){
			if(frames_menuanim==0 || frames_menuanim==5)
				k=k*-1;
			frames_menuanim+= k;
				
	    	lastFrameTime4=currentTime;
	}
	


	//render menu bg frames and logo
	if(play!=1){
		afficher_image(menu_animation_frames[frames_menuanim], ecran);
	}
	else{
		afficher_image(menubg_inv_frames[frames_bg], ecran);
		
	
	}
	afficher_image(menulogo_frames[logo_frames], ecran);
    	// Render each button based on its current animation frame
	afficher_sprite(btn_newgame, ecran, &btn_newgame_frames[frames[0]]);//frames[0] = frames_newgame 
	afficher_sprite(btn_credits, ecran, &btn_credits_frames[frames[1]]);//frames[1] = frames_credits
	afficher_sprite(btn_settings, ecran, &btn_settings_frames[frames[2]]);
        afficher_sprite(btn_howtoplay, ecran, &btn_howtoplay_frames[frames[3]]);
	afficher_sprite(btn_exit, ecran, &btn_exit_frames[frames[4]]);
	
	//display updated cursor (if position changed)
	

	if((play==1 && frames_bg==5 ) || play!=1){
	
		switch(play){
			case 0: break;
			case 1://afficher new game (select slots)
				newgame(ecran, &play, menubg_inv_frames, menubg_frames);
				init_image(&menubg_inv_frames[0], "game/bgmenuf1.jpg",0,0);//change this
				if(play==6){
					*page = 2;
					quitter=0;
					}
				if(play==7){
					*page = 3;
					quitter=0;
					}
				if(play==8){
					*page = 4;
					quitter=0;
					}
				
				break;
			case 2://afficher credits
				afficher_image(credits_parchment, ecran);
				break;
			case 3://afficher settings
				settings(ecran, &play, menubg_inv_frames[0], menutheme, button_click_chunk, &fullscreen);
				printf("play: %d\n", play);
				//while(play==3);
				break;
			case 4://afficher how to play
				afficher_image(menubg_inv_frames[4], ecran);
				afficher_image(htp_parchment,ecran);
				afficher_text(ecran, &txt_back1, "Back");
				if(text_clicked(event, txt_back1))
					play=0;
				
				break;
			case 5:
				clickedYes = 0;
				clickedNo = 0;
				afficher_image(exit_parchment, ecran);
				afficher_text(ecran, &txt_yes, "Yes");
				afficher_text(ecran, &txt_no, "No");
				break;
			default:break;
			
				
		}
	}
	displayCursor(cursorImage,cur_pos, oldCurPosX,oldCurPosY,ecran);

	SDL_Flip(ecran);
	if(SDL_GetTicks() - startLoop <1000/FRAMES_PER_SECOND)
		SDL_Delay((1000/FRAMES_PER_SECOND) - (SDL_GetTicks()-startLoop));
		
	}
	//Fermeture des sous-systèmes audio et texte, libération de la mémoire des ressources images, texte et son.

	for(int i=0;i<6;i++){
		liberer_image(menulogo_frames[i]);
		liberer_image(menubg_inv_frames[i]);
		liberer_image(menubg_frames[i]);
		liberer_image(menu_animation_frames[i]);
	}
	liberer_image(btn_exit);
	liberer_text(&txt_back1);
	liberer_text(&txt_yes);
	liberer_text(&txt_no);
	liberer_image(menu_logo);
	liberer_image(btn_newgame);
	liberer_image(credits_parchment);
	liberer_image(htp_parchment);
	liberer_image(exit_parchment);
	liberer_image(btn_credits);
	liberer_image(btn_settings);
	liberer_image(btn_howtoplay);
	liberer_chunk(button_click_chunk);
	liberer_music(menutheme);
	if (cursorImage!= NULL) {
	    SDL_FreeSurface(cursorImage);
	    cursorImage = NULL;
	} else {
	    fprintf(stderr, "Error: cursorImage is NULL\n");
	}
	//memory leaks and bugs caused by liberer text (valgrind debug) -- fix this


	Mix_CloseAudio();

	
}
