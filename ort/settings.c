#include <SDL/SDL.h>
#include <SDL/SDL_image.h> 
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "fonctions.h"
#include "settings.h"
#include "constants.h"

void settings(SDL_Surface *ecran, int *play, Image menuBg, Mix_Music *music_theme, Mix_Chunk *button_sfx , int *fullscreen){
	int settings_active = 1, i, clickedp1=0,clickedp2=0, clickedm1=0, clickedm2=0, hovered=0,hovered2=0 ,frames_button=0, displaySelected=*fullscreen, controlsSelected=1;
	SDL_Rect cur_pos;
	int muteSound=1, muteMusic=1, oldVolumeMusic=100, oldVolumeSfx=100, arrowCount=-1; //1-not muted 0- muted  [act as frame counter for position]
	SDL_Rect bookVolumePos[2],soundMutePos[2], musicMutePos[2], plusButtonPos[2],minusButtonPos[2],bookMusicRowPos[5], bookSfxRowPos[5];
	int nVolumeMusic,nVolumeSfx, volumeMusic, volumeSfx;
	Image selectButtonAnim;
	Image soundParchment, displayParchmentW, displayParchmentF, controlsParchment1, controlsParchment2, displayParchmentWHovered, displayParchmentFHovered, controlsParchment1Hovered, controlsParchment2Hovered,btn_musicMute, btn_soundMute, book, btn_minus1, btn_plus1, btn_minus2, btn_plus2;
	float oldCurPosX=0, oldCurPosY=0;
	Mix_Chunk *button_clic2 = NULL;
	Text txt_back;
	SDL_Event event;
	SDL_Rect buttonAnimFrames[3];
	SDL_Surface *cursorImage = init_cursor(&cur_pos);
	txt_back = init_text(SCREEN_W*0.88, menuBtnPosY+btnSpacing*4.5, "fonts/emporia-roman.otf", 50, 255, 255, 255); 
	
	loadSettings(&volumeMusic, &volumeSfx);//load volume values from file
	nVolumeMusic = volumeMusic / 25;
	nVolumeSfx = volumeSfx / 25;
	if(volumeMusic==0)
		muteMusic=0;
	if(volumeSfx==0)
		muteSound=0;
		
	button_clic2 = init_chunk("game/button_clic.wav");
        Mix_VolumeChunk(button_clic2, volumeSfx);
		
	init_image(&selectButtonAnim, "game/glowy.png",0,0);
	init_image(&soundParchment, "game/soundParchment.png", SCREEN_W*0.04, SCREEN_H*0.06);
	init_image(&btn_musicMute, "game/musicMuteButton.png", SCREEN_W*0.215, SCREEN_H*0.475);
	init_image(&btn_minus2, "game/buttonminus.png", SCREEN_W*0.18, SCREEN_H*0.485);
	init_image(&btn_plus2, "game/buttonplus.png", SCREEN_W*0.26, SCREEN_H*0.485);
	init_image(&btn_minus1, "game/buttonminus.png", SCREEN_W*0.18, SCREEN_H*0.26);
	init_image(&btn_plus1, "game/buttonplus.png", SCREEN_W*0.26, SCREEN_H*0.26);
	init_image(&btn_soundMute, "game/soundMuteButton.png", SCREEN_W*0.215, SCREEN_H*0.25);
	init_image(&book, "game/bookVolume.png", SCREEN_W*0.02, SCREEN_H*0.06);
	init_image(&displayParchmentF, "game/DisplayFullscreen.png", SCREEN_W*0.04+soundParchment.img->w, SCREEN_H*0.06); 
	init_image(&displayParchmentW, "game/DisplayWindowed.png", SCREEN_W*0.04+soundParchment.img->w, SCREEN_H*0.06+displayParchmentF.img->h); 
	init_image(&controlsParchment1, "game/p3_1.png", SCREEN_W*0.05+soundParchment.img->w+displayParchmentF.img->w, SCREEN_H*0.06); 
	init_image(&controlsParchment2, "game/p3_2.png", SCREEN_W*0.05+soundParchment.img->w+displayParchmentF.img->w, SCREEN_H*0.06+controlsParchment1.img->h); 
	init_image(&displayParchmentFHovered, "game/DisplayFullscreenHovered.png", SCREEN_W*0.04+soundParchment.img->w, SCREEN_H*0.06); 
	init_image(&displayParchmentWHovered, "game/DisplayWindowedHovered.png", SCREEN_W*0.036+soundParchment.img->w, SCREEN_H*0.06+displayParchmentF.img->h); 
	init_image(&controlsParchment1Hovered, "game/controlsParchment1Hovered.png", SCREEN_W*0.05+soundParchment.img->w+displayParchmentF.img->w, SCREEN_H*0.06); 
	init_image(&controlsParchment2Hovered, "game/controlsParchment2Hovered.png", SCREEN_W*0.05+soundParchment.img->w+displayParchmentF.img->w, SCREEN_H*0.06+controlsParchment1.img->h); 
	
	size_image(&btn_musicMute, 55, 55);
	size_image(&btn_soundMute, 55, 55);
	size_image(&book, 100, 110);
	size_image(&btn_plus1, 36, 35);
	size_image(&btn_minus1, 36, 35);
	size_image(&btn_plus2, 36, 35);
	size_image(&btn_minus2, 36, 35);
	
	
	//clip mute buttons sounds
	for(i=0;i<2;i++){
		soundMutePos[i].x=55*i;
		soundMutePos[i].y=0;
		soundMutePos[i].w=55;
		soundMutePos[i].h=55;
		
		musicMutePos[i].x=55*i;
		musicMutePos[i].y=0;
		musicMutePos[i].w=55;
		musicMutePos[i].h=55;
		
		bookVolumePos[i].x=100*i;
		bookVolumePos[i].y=0;
		bookVolumePos[i].w=100;
		bookVolumePos[i].h=110;
		
		plusButtonPos[i].x=36*i;
		plusButtonPos[i].y=0;
		plusButtonPos[i].w=36;
		plusButtonPos[i].h=35;
		
		minusButtonPos[i].x=36*i;
		minusButtonPos[i].y=0;
		minusButtonPos[i].w=36;
		minusButtonPos[i].h=35;
		
		
	}
	for(i=0;i<4;i++){
		buttonAnimFrames[i].x=48*i;
		buttonAnimFrames[i].y=0;
		buttonAnimFrames[i].w=48;
		buttonAnimFrames[i].h= 59;
	}
	// set books rows positions (not clips)
	for(i=0;i<5;i++){
		bookMusicRowPos[i].x=SCREEN_W*0.055+(100*i);
		bookMusicRowPos[i].y=SCREEN_H*0.55;//0.33
		
		bookSfxRowPos[i].x=SCREEN_W*0.055+(100*i);
		bookSfxRowPos[i].y=SCREEN_H*0.33;
	}
	Uint32 lastFrameTime, lastFrameTime2 = SDL_GetTicks();
	while(settings_active){
		clickedm1=0;
		clickedp1=0;
		clickedp2=0;
		clickedm2=0;
		while (SDL_PollEvent(&event)) {
			clickedm1=0;
		clickedp1=0;
		clickedp2=0;
		clickedm2=0;
			switch(event.type){
			case SDL_QUIT: 
			settings_active = 0;
			*play = 0;
			break;
			
			//Read Keyboard event
			case SDL_KEYDOWN:
				//Si on clique sur le bouton echap     		
				switch(event.key.keysym.sym) {
					
					case SDLK_ESCAPE: //quit
						settings_active=0;
						*play = 0;
						break;
					case SDLK_UP:
						if(arrowCount!=0)
							arrowCount--;
						break;
					case SDLK_DOWN:
						arrowCount++;
						break;
					case SDLK_f: //fullscreen toggle
						SDL_WM_ToggleFullScreen(ecran);
						if(*fullscreen==1){
							*fullscreen=2;
							displaySelected=2;
						}
						else{
							*fullscreen=1;
							displaySelected=1;
							}
						break;
					case SDLK_n:
						*play=1;
						settings_active=0;
						break;
					case SDLK_c:
						*play=3;
						settings_active=0;
						break;
					case SDLK_h:
						*play=4;
						settings_active=0;
						break;
					case SDLK_RETURN:
						
						switch(hovered){
							case 1: 
								Mix_PlayChannel(2,button_clic2, 0);
								if(muteSound){
									if(volumeSfx>0)
										volumeSfx -= 25;
									if(volumeSfx==0)
										muteSound=0;
									clickedm1=1;
						
									}
								break;
							case 2:
								Mix_PlayChannel(2,button_clic2, 0);
								if(muteSound){
									oldVolumeSfx = volumeSfx;
									muteSound=0;
									volumeSfx=0;
								}
								else{
									volumeSfx = oldVolumeSfx;
									muteSound=1;
								}
								break;
							case 3:
								Mix_PlayChannel(2,button_clic2, 0);
								if(!muteSound){
									volumeSfx=0;
									muteSound=1;	
								}
								if(volumeSfx<125)
									volumeSfx += 25;
								clickedp1=1;
								break;
							case 4:
								Mix_PlayChannel(2,button_clic2, 0);
								if(muteMusic){
									if(volumeMusic>0)
										volumeMusic -= 25;
									if(volumeMusic==0)
										muteMusic=0;
									clickedm2=1;
									}
								break;
							case 5:
								Mix_PlayChannel(2,button_clic2, 0);
								if(muteMusic){
								oldVolumeMusic=volumeMusic;
								muteMusic=0;
								volumeMusic=0;
							}
							else{
								volumeMusic = oldVolumeMusic;
								muteMusic=1;	
							}
							break;
							case 6:
								Mix_PlayChannel(2,button_clic2, 0);
								if(!muteMusic){
									volumeMusic=0;
									muteMusic=1;
								}
								if(volumeMusic<125)
									volumeMusic += 25;
								clickedp2=1;
								break;
							case 7:
								if(displaySelected!=1){
								if(displaySelected!=1)
									Mix_PlayChannel(2,button_sfx, 0);
								displaySelected=1;
								SDL_WM_ToggleFullScreen(ecran);
								*fullscreen=1;
								}
								break;
							case 8:
								if(displaySelected!=2){
								
								if(displaySelected!=2)
									Mix_PlayChannel(2,button_sfx, 0);
								displaySelected=2;
								*fullscreen=2;
								SDL_WM_ToggleFullScreen(ecran);
								
								}
								break;
							case 9:
								*play = 0;//arjaa lel main menu
								settings_active=0;break;
							//add controls later
								
								
								
							
						}
						break;
					default: break;
				}		
				break;
			
			case SDL_MOUSEBUTTONDOWN:
				if (text_clicked(event, txt_back)) {
					*play = 0;//arjaa lel main menu
					settings_active=0;
				}
				if(button_clicked(event, btn_soundMute)){
					Mix_PlayChannel(2,button_clic2, 0);
					if(muteSound){
						oldVolumeSfx = volumeSfx;
						muteSound=0;
						volumeSfx=0;
					}
					else{
						volumeSfx = oldVolumeSfx;
						muteSound=1;
					}
				}
				if(button_clicked(event, btn_musicMute)){
					Mix_PlayChannel(2,button_clic2, 0);
					if(muteMusic){
						oldVolumeMusic=volumeMusic;
						muteMusic=0;
						volumeMusic=0;
					}
					else{
						volumeMusic = oldVolumeMusic;
						muteMusic=1;	
					}
				}
				if(button_clicked(event, btn_minus1)){
					Mix_PlayChannel(2,button_clic2, 0);
					if(muteSound){
					if(volumeSfx>0)
						volumeSfx -= 25;
					if(volumeSfx==0)
						muteSound=0;
					clickedm1=1;
					
					}
				}
				else if(button_clicked(event, btn_plus1)){
					Mix_PlayChannel(2,button_clic2, 0);
					if(!muteSound){
						volumeSfx=0;
						muteSound=1;	
					}
					if(volumeSfx<125)
						volumeSfx += 25;
					clickedp1=1;
				}
				
				if(button_clicked(event, btn_minus2)){
					Mix_PlayChannel(2,button_clic2, 0);
					if(muteMusic){
					if(volumeMusic>0)
						volumeMusic -= 25;
					if(volumeMusic==0)
						muteMusic=0;
					clickedm2=1;
					}
				}
				else if(button_clicked(event, btn_plus2)){
					Mix_PlayChannel(2,button_clic2, 0);
					if(!muteMusic){
						volumeMusic=0;
						muteMusic=1;
					}
					if(volumeMusic<125)
						volumeMusic += 25;
					clickedp2=1;
				}
				saveSettings(volumeMusic, volumeSfx);
				
				if(displaySelected!=2 && button_clicked(event, displayParchmentW)){
					if(displaySelected!=2)
						Mix_PlayChannel(2,button_sfx, 0);
					displaySelected=2;
					*fullscreen=2;
					SDL_WM_ToggleFullScreen(ecran);
				}
				else if(displaySelected!=1 && button_clicked(event, displayParchmentF)){
					if(displaySelected!=1)
						Mix_PlayChannel(2,button_sfx, 0);
					displaySelected=1;
					*fullscreen=1;
					SDL_WM_ToggleFullScreen(ecran);
				}
				if(controlsSelected!=2 && button_clicked(event, controlsParchment1)){
					if(controlsSelected!=1)
						Mix_PlayChannel(2,button_sfx, 0);
					controlsSelected=1;
				}
				else if(controlsSelected!=1 && button_clicked(event, controlsParchment2)){
					if(controlsSelected!=2)
						Mix_PlayChannel(2,button_sfx, 0);
					controlsSelected=2;
				}
				break;
			case SDL_MOUSEMOTION:
				arrowCount=-1;
				updateCursorPosition(&oldCurPosX, &oldCurPosY, &cur_pos, event);
				hovered2=0;
				if(button_hovered(event, displayParchmentF))
					hovered2=1;
				else if(button_hovered(event, displayParchmentW))
					hovered2=2;
				else if(button_hovered(event, controlsParchment1))
					hovered2=3;
				else if(button_hovered(event, controlsParchment2))
					hovered2=4;
				else if (text_hovered(event, txt_back)){
					hovered2=5;
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
				hovered=5;break;
			case 5:
				hovered=6;break;
			case 6: 
				hovered=7;break;
			case 7:
				hovered=8;break;
			case 8: hovered=9;break;
			case 9:
				hovered=1;
				arrowCount=0;
				break;
			}
		}
		
		if(hovered2!=5)
			frames_button=0;
		Uint32 currentTime = SDL_GetTicks();
		if (currentTime - lastFrameTime2 > 100){		
			if (hovered2 ==5)
			{    
				if (frames_button < 2) {
				    frames_button++;
				}
		
	    		}

		    	lastFrameTime2=currentTime;
		}

		//music control and change values
		nVolumeMusic = volumeMusic / 25;
		nVolumeSfx = volumeSfx / 25;
		Mix_VolumeChunk(button_sfx, volumeSfx);
		Mix_VolumeMusic(volumeMusic);	
		Mix_VolumeChunk(button_clic2, volumeSfx);	
		//display
		afficher_image(menuBg, ecran);
		afficher_image(soundParchment, ecran);
		afficher_image(displayParchmentF, ecran);
		afficher_image(displayParchmentW, ecran);
		afficher_image(controlsParchment1, ecran);
		afficher_image(controlsParchment2, ecran);
		if(displaySelected==1)
			afficher_image(displayParchmentFHovered, ecran);
		else if(displaySelected==2)
			afficher_image(displayParchmentWHovered, ecran);
		
		if(controlsSelected==1)
			afficher_image(controlsParchment1Hovered, ecran);
		else if(controlsSelected==2)
			afficher_image(controlsParchment2Hovered, ecran);
		afficher_sprite(btn_soundMute, ecran ,&soundMutePos[muteSound]);
		afficher_sprite(btn_musicMute, ecran ,&musicMutePos[muteMusic]);
		afficher_sprite(btn_minus1, ecran , &minusButtonPos[clickedm1]);		
		afficher_sprite(btn_plus1, ecran , &plusButtonPos[clickedp1]);
		afficher_sprite(btn_minus2, ecran , &minusButtonPos[clickedm2]);
		afficher_sprite(btn_plus2, ecran , &plusButtonPos[clickedp2]);
		for(i=0;i<5;i++){
			afficher_sprite_at_pos(book, ecran, &bookVolumePos[1], &bookMusicRowPos[i]);
			afficher_sprite_at_pos(book, ecran, &bookVolumePos[1], &bookSfxRowPos[i]);
		}
		for(i=0;i<nVolumeMusic;i++){
			afficher_sprite_at_pos(book, ecran, &bookVolumePos[0], &bookMusicRowPos[i]);
		}
		for(i=0;i<nVolumeSfx;i++){
			afficher_sprite_at_pos(book, ecran, &bookVolumePos[0], &bookSfxRowPos[i]);
		}
		afficher_text(ecran, &txt_back, "Back");
		switch(hovered2){
			case 0:break;
			case 1:afficher_image(displayParchmentFHovered, ecran);break;
			case 2: afficher_image(displayParchmentWHovered, ecran);break;
			case 3: afficher_image(controlsParchment1Hovered, ecran);break;
			case 4: afficher_image(controlsParchment2Hovered, ecran);break;
			case 5:
				changeImagePos(&selectButtonAnim, txt_back.pos_txt);	
				afficher_sprite(selectButtonAnim, ecran, &buttonAnimFrames[frames_button]);
				break;
			default:break;
		}
		displayCursor(cursorImage,cur_pos, oldCurPosX,oldCurPosY,ecran);
		SDL_Flip(ecran);
	}

	liberer_text(&txt_back);
	liberer_image(book);
	liberer_image(btn_minus1);
	liberer_image(btn_minus2);
	liberer_image(btn_plus1);
	liberer_image(btn_plus2);
	liberer_image(btn_soundMute);
	liberer_image(btn_musicMute);
	liberer_image(soundParchment);
	liberer_image(displayParchmentF);
	liberer_image(displayParchmentW);
	liberer_image(controlsParchment1);
	liberer_image(controlsParchment2);
	liberer_chunk(button_clic2);
	liberer_image(selectButtonAnim);
	liberer_image(displayParchmentWHovered); 
	liberer_image(displayParchmentFHovered);
	liberer_image(controlsParchment1Hovered);
	liberer_image(controlsParchment2Hovered);
	SDL_FreeSurface(cursorImage);


}



	
