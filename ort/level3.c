#include <SDL/SDL.h>
#include <SDL/SDL_image.h> 
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <string.h>
#include "fonctions.h"
#include "settings.h"
#include "newgame.h"
#include "constants.h"
#include "memorygame.h"
#include "dialogue.h"
#include "Ortcrush/ortcrushmain.h"
#include "player.h"
#include "collision.h"
#include "minimap.h"
#include "scrolling.h"
#include "enigme/enigme.h"
#include "tictactoe/tic.h"
#include "particle_system.h"
#include "npc.h"
#include "save_system.h"
#include "serie.h"
#include "enemy.h"
#include "gameObject.h"
#include "tutorial.h"
#include "document.h"


void game_level3(SDL_Surface *ecran, int *page){

	//INIT CONTROLLER INTEGRATION----
	char buffer[1];                   // un buffer
	int j;

	// ouverture du port à 115200 bauds
	//int fd = serialport_init("/dev/ttyUSB0", 9600);
	//if (fd==-1) return -1;
	
	
	//FIN INIT CONTROLLER INTEGRATION----
	
	
	//INIT LEVEL SAVE SYSTEM AND LOAD IF POSSIBLE-------
	GameData level1data;
	//int level1loaded = loadLevelData(1, &level1data);
	int level1loaded = 0;
	//FIN INIT SAVE SYSTEM
     
	
	
	printf("entered game level1\n");
	int score=0;
	char scoretotxt[10];
	Image timerbg, background , background2, pausemenuspritesheet, pausemenu, resume, restart, settings, exit, pausemenubg[2], staminabar, healthicon;
	Image dustparticle, healthparticle;
	SDL_Rect cur_pos, pausemenuframespos[8];
	Objective obj[3];
	int dialogueActive=0, pausemenuframes[4], pausemenubgframes=0, hoveredPauseMenu=-1;
	Image hb[5], memorygame[6], rightline, intro_img_spritesheet, blackbartop, blackbarbottom;
	SDL_Rect intro[4], memorygamerect[6];
	int level_active=1, pausemenuActive=0,memorygameactive=0, selectedcard1=-1, selectedcard2=-1, previouscard1=-1, previouscard2=-1, lastrightcard1=-1, lastrightcard2=-1,memorygamescore=0;
	int memorygame_answers[6], cardsframes[6], reward,memorygame_flipped[6];  //memorygame_flipped makes sure the player doesnt flip the answered cards
	char timerTxt[40]="";
	int obj_hovered=0, obj_hovered2=0, obj_hovered3=0;//declare new ones if i want to add more objectives
	int intro_frames=0;
	int game_time=150;
	int game_time_const = game_time;
	int game_status=0; //0- playing  s1- lost  2- won
	float oldCurPosX=0, oldCurPosY=0;
	int minutes=game_time/60, seconds=game_time%60, i;
	int volumeMusic, volumeSfx;
	Uint32 startTicks, currentTick, timer=0, oldTimer=0, start=0, currentAnimTime, lastFrameTime=0;//fps lock and timer vars
	//Uint32 dt, t_prev;//player vars
	Mix_Music* level2theme = NULL;
	Mix_Chunk*health_hit_sfx=NULL, *collect_sfx=NULL, *heal_sfx=NULL, *cardflip_sfx=NULL, *dash_sfx=NULL;

	
	
	//init cursor and text
	SDL_Surface *cursorImage4 = init_cursor(&cur_pos);
	Text timerSurface = init_text(SCREEN_W*0.457, SCREEN_H*0.117, "fonts/emporia-roman.otf", 65, 255, 255, 255);
	Text timewordSurface = init_text(SCREEN_W*0.475, SCREEN_H*0.08, "fonts/emporia-roman.otf", 30, 255, 255, 0);
	Text txt_returnmenu = init_text(SCREEN_W*0.458, SCREEN_H*0.25, "fonts/emporia-roman.otf", 50, 255, 255, 255); 
	Text txt_hit = init_text(SCREEN_W*0.458, SCREEN_H*0.4, "fonts/emporia-roman.otf", 50, 255, 255, 255); 
	Text txt_tictactoe = init_text(SCREEN_W*0.458, SCREEN_H*0.5, "fonts/emporia-roman.otf", 50, 255, 255, 255);  
	Text txt_collect = init_text(SCREEN_W*0.458, SCREEN_H*0.6, "fonts/emporia-roman.otf", 50, 255, 255, 255);
	Text txt_ortcrush = init_text(SCREEN_W*0.458, SCREEN_H*0.7, "fonts/emporia-roman.otf", 50, 255, 255, 255);  
	Text txt_enigme = init_text(SCREEN_W*0.458, SCREEN_H*0.8, "fonts/emporia-roman.otf", 50, 255, 255, 255);  
	Text txt_playerScore= init_text(100, 400, "fonts/emporia-roman.otf", 50, 255, 255, 255);
	SDL_Event event;
	//init music
	if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048)==-1)
	{
		printf("Error Opening Audio[music|chunk]: %s \n", Mix_GetError());
		Mix_CloseAudio();
		return ;
	}  
	Mix_Chunk *gameover_sfx = init_chunk("game/gameover_sfx.wav");
	//load music
	loadSettings(&volumeMusic, &volumeSfx);
        init_music(level2theme, "game/industrial rev level3.wav");
        //set volume for music
	Mix_VolumeMusic(volumeMusic);
	//load chunk
	health_hit_sfx= init_chunk("game/health_hit_sfx.wav");
	collect_sfx= init_chunk("game/select_sfx.wav");
	heal_sfx = init_chunk("game/heal_sfx.wav");
	cardflip_sfx= init_chunk("game/cardflipsfx.wav");
	dash_sfx= init_chunk("game/dash_sfx.wav");
        if(health_hit_sfx==NULL || collect_sfx==NULL)
        	printf("\n%s | %s \n", SDL_GetError(), Mix_GetError());
        //set volumes for chunk
        Mix_VolumeChunk(health_hit_sfx, volumeSfx);
        Mix_VolumeChunk(collect_sfx, volumeSfx);
        Mix_VolumeChunk(heal_sfx, volumeSfx);
        Mix_VolumeChunk(cardflip_sfx, volumeSfx);
        Mix_VolumeChunk(dash_sfx, volumeSfx);
        //init chapter intro
        init_image(&intro_img_spritesheet, "game/intro_lvl3_spritesheet.png", SCREEN_W-984, SCREEN_H*0.27);
        init_image(&blackbartop, "game/blackbar.png", 0, 0);
        init_image(&blackbarbottom, "game/blackbar.png", 0,SCREEN_H-105 );
        
        for(i=0;i<4;i++){
        	intro[i].x=984*i;
		intro[i].y=0;
		intro[i].w=984;
		intro[i].h= 472;
        
        } 
        
        
        //init pause menu| free them later
        init_image(&pausemenubg[0], "game/pausemenubgf1.png", 0, 0);
        init_image(&pausemenubg[1], "game/pausemenubgf2.png", 0, 0);  
        init_image(&pausemenu, "game/pausemenupausedtxt.png", SCREEN_W*0.84, 0);
        init_image(&resume, "game/pausemenuspritesheet.png", SCREEN_W*0.1, SCREEN_H*0.20);
        init_image(&restart, "game/pausemenuspritesheet.png", SCREEN_W*0.1, SCREEN_H*0.40);
        init_image(&settings, "game/pausemenuspritesheet.png", SCREEN_W*0.1, SCREEN_H*0.60);
        init_image(&exit, "game/pausemenuspritesheet.png",SCREEN_W*0.1, SCREEN_H*0.80); 
        Text txt_resume= init_text(resume.pos1.x+resume.pos1.x*0.80, resume.pos1.y+resume.pos1.y*0.18, "fonts/BerlinSansFBRegular.ttf", 70, 64, 58, 41);     
        Text txt_restart= init_text(resume.pos1.x+resume.pos1.x*0.77, restart.pos1.y+restart.pos1.y*0.09, "fonts/BerlinSansFBRegular.ttf", 70, 64, 58, 41);     
        Text txt_settings= init_text(resume.pos1.x+resume.pos1.w*0.23, settings.pos1.y+settings.pos1.y*0.07, "fonts/BerlinSansFBRegular.ttf", 70, 64, 58, 41);
        Text txt_exit= init_text(resume.pos1.x+resume.pos1.w*0.35, exit.pos1.y+exit.pos1.y*0.07, "fonts/BerlinSansFBRegular.ttf", 70, 64, 58, 41);  
        Text info = init_text(100, 200, "fonts/arial.ttf", 40,  0, 0, 0);
        Text info2 = init_text(100, 350, "fonts/arial.ttf", 40,  0, 0, 0);  
        Text info3 = init_text(100, 450, "fonts/arial.ttf", 40,  0, 0, 0);  
        Text info4 = init_text(100, 550, "fonts/arial.ttf", 40,  0, 0, 0);  
        Text info5 = init_text(100, 650, "fonts/arial.ttf", 40,  0, 0, 0);  
        Text info6 = init_text(100, 750, "fonts/arial.ttf", 40,  0, 0, 0);        
        
        for(i=0;i<8;i++){
		//clip sprites row
		pausemenuframespos[i].x=0;
		pausemenuframespos[i].y=155*i;
		pausemenuframespos[i].w=610;
		pausemenuframespos[i].h= 144;
	}
	
	for(i=0;i<4;i++){
		pausemenuframes[i]=0;
	}
        //init UI
        Image scroll_btn, tutorial_btn, gamewon_img;
        init_image(&scroll_btn, "game/gamewon_lvl2.png", SCREEN_W*0.9, SCREEN_H * 0.2);
        init_image(&scroll_btn, "game/scroll_btn.png", SCREEN_W*0.9, SCREEN_H * 0.2);
        init_image(&tutorial_btn, "game/info_btn.png", SCREEN_W*0.9, SCREEN_H * 0.28);
	init_image(&timerbg, "game/timerbgo50.png",SCREEN_W*0.4476,SCREEN_H*0.024);
	init_image(&rightline, "game/staminabarempty.png", SCREEN_W*0.54, SCREEN_H*0.05); 
	init_image(&staminabar, "game/staminabarorange.png", SCREEN_W*0.612, SCREEN_H*0.0860); 
	//init health bar
	init_image(&hb[0], "game/hb_blue.png", SCREEN_W*0.1, SCREEN_H*0.064); 
	init_image(&hb[1], "game/hb_green.png", SCREEN_W*0.1, SCREEN_H*0.064); 
	init_image(&hb[2], "game/hb_orange.png", SCREEN_W*0.1, SCREEN_H*0.064); 
	init_image(&hb[3], "game/hb_red.png", SCREEN_W*0.1, SCREEN_H*0.064);
	init_image(&hb[4], "game/hb_empty.png", SCREEN_W*0.1, SCREEN_H*0.064); 
	init_image(&healthicon, "game/hpicon.png", hb[4].pos1.x+hb[4].pos1.w*0.77, SCREEN_H*0.052); 
	//init objectives
	init_objective(&obj[0], "Reach the Top" , 1, 0.3*SCREEN_H);
	init_objective(&obj[1], "Win 1 minigame" , 1, 0.35*SCREEN_H);
	init_objective(&obj[2], "Collect 3 Documents" , 3, 0.4*SCREEN_H);
	
	//init memory game
        init_memorygame_spritesheet(memorygamerect);//clip memory game spritesheet 
        init_memorygame(memorygame);//init images array of memory game cards in the correct pos
        init_memorygame_card_frames(cardsframes);//init all cards frames to 0
        init_memorygame_answers(memorygame_answers);//testing
        for(int i =0;i<6;i++)
        	memorygame_flipped[i] = 0;
        

        
        
        //PARTICLE SYSTEM INIT
        Particle dustparticles[MAX_PARTICLES];
        Particle healthparticles[MAX_PARTICLES];
        init_image(&dustparticle, "game/particle4.png", 0, 0);
        init_image(&healthparticle, "game/health_particle.png", 0, 0);
        initParticleSystems(dustparticles, dustparticle.img, dustparticle.img->w, dustparticle.img->h);
        initParticleSystems(healthparticles, healthparticle.img, healthparticle.img->w, healthparticle.img->h);
        
        
        ////////////INTEGRATION 
        Uint32 t_prev, dt=0, lastframetime2=0;
	Image bg, bgmask;
	Camera camera;
	Minimap m;
	Player p1;
	int animate, gameover=0, gamewon=0;
        Uint32 currenttime;
       	//init_image(&bg, "bgminimaptest1500v2.jpg",0,0);
	//init_image(&bgmask, "bgminimaptest1500maskedblackground.jpg", 0, 0);
	init_image(&bg, "level3bg.png",0,0);
	init_image(&bgmask, "level3bgmask.png", 0, 0);
	init_minimapVertical(&m, 100, 110, (SDL_Rect){100, 5000}, "game/level3minimap.png");
	Indicator ind_enigma1 = add_minimap_indicator("enigma", 800, 300, 3, m);
	Indicator ind_enigma2 = add_minimap_indicator("enigma", 1400, 800, 3, m);
	if(level1loaded){
		printf("LEVEL1 LOADEDD\n\n\n\n");
		init_player(&p1, level1data.playerX, level1data.playerY, 4 ,10);
		p1.hp = level1data.playerHP;	
		printf("HP: %d\n", p1.hp);
	}
	else{
		init_player(&p1, 1500, 5000, 8 ,14);//8->num max frames| 14>num movements/rows
		printf("HP: %d\n", p1.hp);
	}
        Image *playerspritesheets=NULL;
	playerspritesheets = (Image *)malloc(p1.max_rows * sizeof(Image));
	if(playerspritesheets==NULL){
		printf("MEMORY ALLOCATION ERROR\n");
		return;	
	}
	init_image(&playerspritesheets[0], "idle_right.png", 0,0);
	init_image(&playerspritesheets[1], "walk_right.png", 0,0);
	init_image(&playerspritesheets[2], "run_right.png", 0,0);
	init_image(&playerspritesheets[3], "jump_right.png", 0,0);
	init_image(&playerspritesheets[4], "idle_left.png", 0,0);
	init_image(&playerspritesheets[5], "walk_left.png", 0,0);
	init_image(&playerspritesheets[6], "run_left.png", 0,0);
	init_image(&playerspritesheets[7], "jump_left.png", 0,0);
	init_image(&playerspritesheets[8], "dash_right.png", 0,0);
	init_image(&playerspritesheets[9], "dash_left.png", 0,0);
	init_image(&playerspritesheets[10], "fight_right.png", 0,0);
	init_image(&playerspritesheets[11], "fight_left.png", 0,0);
	init_image(&playerspritesheets[12], "death_right.png", 0,0);
	init_image(&playerspritesheets[13], "death_left.png", 0,0);
	init_image(&p1.deathscreen, "deathscreen.png", 0, 0);
	//init_image(&pfightsheet ,"fight_right.png", 0, 0);
	init_player_spritesheet(&p1, playerspritesheets);
	if(level1loaded)
		initCamera(&camera, level1data.cameraX, level1data.cameraY, 1920, 1080, 1);
	else
		initCamera(&camera, 0, p1.pos.y + p1.playerH / 2 - 1080/2, 1920, 1080, 1);
	Uint8 *keystate = SDL_GetKeyState(NULL);
	Image *obstacles = NULL;
	obstacles = (Image *)malloc(4*sizeof(Image));//update 4 to num obstacles
	if(obstacles==NULL)
		printf("MEMORY ALLOCATION ERROR\n");
	Image mainplatform, squaredplatform, greenobstacle, standingplatform;//move this
	init_image(&mainplatform, "game/bgmainplatform.png",0, 930);
	init_image(&standingplatform, "standingplatform.png",1140, 490);
	init_image(&greenobstacle, "greenobstacle.png",1600, 1300);
	init_image(&squaredplatform, "squaredplatform.png",1140, 850);
	obstacles[0] = mainplatform;
	obstacles[1] = standingplatform;
	obstacles[2] = greenobstacle;
	obstacles[3] = squaredplatform;
	t_prev = SDL_GetTicks();
        
        
        //-------NPC INIT--------
        int n_npc=0;
      //NPC JOBS: 1-minigame ortcrush 2- tictactoe 3- Q&A 4- minigame memory card 5- INFORMATIONAL NPC(Only gives information and documents)
        Npc npcs[n_npc];
       /* npcs[0] = init_npc("npc_spritesheet_level2.png", 1 , 208, 646, 1);
        npcs[1] = init_npc("npc_spritesheet_level2.png", 3 , 3463, 646, 2);
        npcs[2] = init_npc("npc_spritesheet_level2.png", 5 , 4593, 410, 3);
        npcs[3] = init_npc("npc_spritesheet_level2.png", 1 , 7900, 455, 4);
*/

        //---------------------------
        
        
        //---------ENEMY INIT--------
        int n_enemy=0;
        Enemy e[n_enemy];
        //init_enemy(&e[0], 1100, 340);
      /*  init_enemy(&e[1], 3405, 512);
        init_enemy(&e[2], 4080, 552);
        init_enemy(&e[3], 5120, 570);
        init_enemy(&e[4], 5469, 543);
        init_enemy(&e[5], 6263, 538);
        init_enemy(&e[6], 6962, 570);
        init_enemy(&e[7], 4380, -70);*/
        //FIN ENEMY INIT -------
        
        //---------GAMEOBJECT INIT--------
        int n_gameObj=0;
        GameObject *gameObjects = NULL;
        gameObjects = (GameObject *)malloc(n_gameObj*sizeof(GameObject));
      /*/  init_gameObject(&gameObjects[0], 475, 457, "enigma_minimap_indicator.png",2);
        init_gameObject(&gameObjects[1], 427, 457, "game/pen_indicator.png",8);
        init_gameObject(&gameObjects[2], 568, 263, "game/book_indicator.png",9);
        init_gameObject(&gameObjects[3], 613, 263, "game/pen_indicator.png",8);
        init_gameObject(&gameObjects[4], 677, 263, "game/pen_indicator.png",8);
        init_gameObject(&gameObjects[5], 774, 282, "game/pen_indicator.png",8);
        init_gameObject(&gameObjects[6], 900, 643, "game/pen_indicator.png",8);
        init_gameObject(&gameObjects[7], 937, 643, "game/pen_indicator.png",8);
        init_gameObject(&gameObjects[8], 730, 746, "enigma_minimap_indicator.png",3);
        init_gameObject(&gameObjects[9], 1261, 523, "game/time_indicator.png",6);
        init_gameObject(&gameObjects[10], 1173, 523, "game/book_indicator.png",9);
        init_gameObject(&gameObjects[11], 1095, 523, "game/book_indicator.png",9);
        init_gameObject(&gameObjects[12], 1477, 746, "game/scroll_indicator_spritesheet.png",7);
        init_gameObject(&gameObjects[13], 1636, 663, "game/pen_indicator.png",8);
        init_gameObject(&gameObjects[14], 1812, 487, "game/book_indicator.png",9);
        init_gameObject(&gameObjects[15], 1902, 406, "game/pen_indicator.png",8);
        init_gameObject(&gameObjects[16], 1974, 406, "game/time_indicator.png",6);
        init_gameObject(&gameObjects[17], 2050, 406, "game/book_indicator.png",9);
        init_gameObject(&gameObjects[18], 2124, 406, "game/book_indicator.png",9);
        init_gameObject(&gameObjects[19], 2307, 411, "enigma_minimap_indicator.png",1);
        init_gameObject(&gameObjects[20], 2541, 235, "game/pen_indicator.png",8);
        init_gameObject(&gameObjects[21], 2745, 459, "game/scroll_indicator_spritesheet.png",7);
        init_gameObject(&gameObjects[22], 2918, 107, "game/pen_indicator.png",8);
        init_gameObject(&gameObjects[23], 3141, 341, "game/book_indicator.png",9);
        init_gameObject(&gameObjects[24], 3000, 746, "game/pen_indicator.png",8);
        init_gameObject(&gameObjects[25], 3128, 746, "game/book_indicator.png",9);
        init_gameObject(&gameObjects[26], 3198, 746, "game/book_indicator.png",9);
        init_gameObject(&gameObjects[27], 3304, 746, "game/pen_indicator.png",8);
        init_gameObject(&gameObjects[28], 3922, 746, "game/pen_indicator.png",8);
        init_gameObject(&gameObjects[29], 4045, 807, "game/pen_indicator.png",8);
        init_gameObject(&gameObjects[30], 4666, 853, "game/scroll_indicator_spritesheet.png",7);
        init_gameObject(&gameObjects[31], 4748, 853, "game/time_indicator.png",6);
        init_gameObject(&gameObjects[32], 5014, 654, "game/book_indicator.png",9);
        init_gameObject(&gameObjects[33], 5291, 546, "game/pen_indicator.png",8);
        init_gameObject(&gameObjects[34], 5400, 546, "game/pen_indicator.png",8);
        init_gameObject(&gameObjects[35], 5829, 649, "game/book_indicator.png",9);
        init_gameObject(&gameObjects[36], 5914, 649, "game/pen_indicator.png",8);
        init_gameObject(&gameObjects[37], 6271, 547, "game/hp_indicator.png",5);
        init_gameObject(&gameObjects[38], 6414, 547, "enigma_minimap_indicator.png",3);
        init_gameObject(&gameObjects[39], 6513, 547, "game/book_indicator.png",9);
        init_gameObject(&gameObjects[40], 6695, 655, "game/scroll_indicator_spritesheet.png",7);
        init_gameObject(&gameObjects[41], 6785, 655, "game/pen_indicator.png",8);
        init_gameObject(&gameObjects[42], 7012, 555, "game/time_indicator.png",6);
        init_gameObject(&gameObjects[43], 7126, 555, "game/book_indicator.png",9);
        init_gameObject(&gameObjects[44], 7269, 561, "game/pen_indicator.png",8);
        init_gameObject(&gameObjects[45], 7415, 555, "enigma_minimap_indicator.png",2);*/
        

          //init_gameObject(&gameObjects[20], 2101, 0, "game/influence_indicator.png",4);

        
        //FIN GAMEOBJECT INIT -------
        
	//-------INIT TUTORIAL SYSTEM
	Tutorials tutorials;
	int n_tuto=5;
	init_tutorial(&tutorials, n_tuto);
	tutorials.T = NULL;
	tutorials.T = (Tuto *)malloc(tutorials.n* sizeof(Tuto));
	if(tutorials.T == NULL){
		printf("erreur allocation tutorial array\n");
		return;
	}
	
	init_tuto(&tutorials.T[0], "tutorials/tutorial_ui.png", 1);
	init_tuto(&tutorials.T[1], "tutorials/tutorial_kb.png", 2);
	init_tuto(&tutorials.T[2], "tutorials/tutorial_obstacles.png", 3);
	init_tuto(&tutorials.T[3], "tutorials/tutorial_npc.png", 4);
	init_tuto(&tutorials.T[4], "tutorials/tutorial_gameobject.png", 5);

	for(int i=0;i<n_tuto;i++)
		activate_tuto(&tutorials);
	
	//dont activate tutorials in this level, if player wants to check tutorials he can press on the button
	
	
	
	
	//FIN TUTORIAL INIT
	
	//----INIT DOCUMENT SYSTEM
	Documents documents;
	init_documents(&documents, 4);
	documents.T = NULL;
	documents.T = (Doc *)malloc(documents.n* sizeof(Doc));
	if(documents.T == NULL){
		printf("erreur allocation documents array\n");
		return;
	}
	
	init_doc(&documents.T[0], "documents/newspaper_1.png", 1,"documents/renaisassy1_voice.wav");
	init_doc(&documents.T[1], "documents/newspaper_2.png", 2,"documents/renaisassy2_voice.wav");
	init_doc(&documents.T[2], "documents/newspaper_3.png", 3,"documents/renaisassy3_voice.wav");
	init_doc(&documents.T[3], "documents/newspaper_4.png", 4,"documents/renaisassy4_voice.wav");


	
	//FIN DOCUMENT INIT
        //------------
	while(level_active)
	{
	/*integration manette
	 serialport_read_until(fd, buffer, '\r', 1, 10);

        // suppression de la fin de ligne
        for (j=0 ; buffer[j]!='\r' && j<3 ; j++);
        buffer[j] = 0;



        // écriture du résultat
        printf("%s", buffer);
        if(strstr(buffer,"r")){
       // Uint32 dx = 0.5 * p1.vitesse* dt*dt + p1.acceleration*dt;
		p1.pos.x += 4;//dx/1000.0;
        }*/
	startTicks = SDL_GetTicks();
	if(game_time>0 && !pausemenuActive)
		timer = SDL_GetTicks();
	start = SDL_GetTicks();
	//t_prev=SDL_GetTicks();
	while (SDL_PollEvent(&event)) {
	
			switch(event.type){
				case SDL_QUIT:
					level_active = 0;
					*page= 0;
					break;
				
				//Read Keyboard event
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_e:
							if (!pausemenuActive && memorygameactive == 0 && intro_frames==4) { 
								for(int k=0;k<n_npc;k++)//make this more optimized
									npcOnInteraction(&npcs[k], ecran, event);
								break;
								
							    }
							break;
						case SDLK_ESCAPE: //quit
							if(!pausemenuActive && intro_frames==4&& p1.status != PLAYER_DEAD){
								for(i=0;i<4;i++){
									pausemenuframes[i]=0;
									}									
								pausemenuActive=1;
								pausemenubgframes=0;
							}
							else
								pausemenuActive=0;
							break;

						default: break;
					}
					if(!pausemenuActive && intro_frames>=4 && p1.status != PLAYER_DEAD && !tutorials.active){	
						handlePlayerKeyDownEvent(&p1, event, keystate);    
						if(p1.isDashing){
							Mix_PlayChannel(2,dash_sfx, 0);	 	
						}	
						}
					break;
				case SDL_KEYUP:
				      if(!pausemenuActive && intro_frames>=4 && p1.status != PLAYER_DEAD)
				      		handlePlayerKeyUpEvent(&p1, event, keystate);
				      break;
				case SDL_MOUSEBUTTONDOWN:
					
					if(pausemenuActive){
						if(text_clicked(event, txt_resume))
							pausemenuActive=0;//resume the game
						else if(text_clicked(event, txt_exit)){
							level_active=0;
							*page=1;//return to main menu
							}
						else if(text_clicked(event, txt_settings)){
							//int fullscreen=0;//pass fullscreen variable through function or save it inside a file when a change happens and load it in each level
							//settings(ecran, int *play, Image menuBg, Mix_Music *music_theme, Mix_Chunk *button_sfx , fullscreen);
						}
					
					}
					else{
					if(memorygameactive==1){
						for(i=0;i<6;i++){
							if(button_clicked(event, memorygame[i]) && memorygame_flipped[i]==0){
								if(selectedcard1 ==-1){
									selectedcard1 = i;
									Mix_PlayChannel(3,cardflip_sfx, 0);
								}
								else if(selectedcard2==-1 && i!=selectedcard1){
									selectedcard2 = i;
									Mix_PlayChannel(3,cardflip_sfx, 0);
									}
							}
						}
					}
					else{
					if(text_clicked(event, txt_hit)){
						if(p1.hp>0){//test hp update
							p1.hp--;
							Mix_PlayChannel(2,health_hit_sfx, 0);
							if(p1.hp==0)
								p1.status = PLAYER_DEAD;
							//emitParticlesExplosion(healthparticles,  hb[4-hp].pos1.x+hp*140, hb[4-hp].pos1.y+40, 10, 5, 2);
							}
					
					}
					if (text_clicked(event, txt_returnmenu)) {
						//level_active=0;
						//testing
						memorygameactive=1;
						}
					if(text_clicked(event, txt_collect)){
						update_objective(&obj[0], 2);//test  objective update
						Mix_PlayChannel(3,collect_sfx, 0);
					}
					if(text_clicked(event, txt_ortcrush)){
						startortcrushgame(ecran, 60);
					}
					if(text_clicked(event, txt_tictactoe)){
						//start tictactoe;
						//starttic(ecran);
						//activate_tuto(&tutorials);
					}
					if(button_clicked(event, tutorial_btn)){
						//activate_tuto(&tutorials);
						if(!tutorials.active && tutorials.currentIndexDisplaying < tutorials.currentLastActiveIndex){
							tutorials.currentIndexDisplaying = 0;//put display on first page if there is at least one active page
							tutorials.active=1;
						}
					}
					if(button_clicked(event, scroll_btn)){
						//activate_tuto(&tutorials);
						if(!documents.active && documents.currentIndexDisplaying < documents.currentLastActiveIndex){
							documents.currentIndexDisplaying = 0;//put display on first page if there is at least one active page
							documents.active=1;
						}
					}
					if(text_clicked(event, txt_enigme)){
						//start enigme
						enigme(ecran);
					}
					
					
					}
					}
				    handleTutoEvent(&tutorials, event);//only works if next tutorial is active 
				    handleDocEvent(&documents, event);
				    break;

				case SDL_MOUSEMOTION:
					updateCursorPosition(&oldCurPosX, &oldCurPosY, &cur_pos, event);
					obj_hovered=0;
					obj_hovered2=0;
					obj_hovered3=0;
					hoveredPauseMenu=-1;
					if(button_hovered(event , obj[0].img)){
						obj_hovered=1;
					}
					if(button_hovered(event , obj[1].img)){
						obj_hovered2=1;
					}
					if(button_hovered(event , obj[2].img)){
						obj_hovered3=1;
					}
					//pause menu animation
					if(pausemenuActive){
					
						if(text_hovered(event, txt_resume))
							hoveredPauseMenu=0;
						if(text_hovered(event, txt_restart))
							hoveredPauseMenu=1;
						if(text_hovered(event, txt_settings))
							hoveredPauseMenu=2;
						if(text_hovered(event, txt_exit))
							hoveredPauseMenu=3;
						}
					break;
			
			}
		}
		
	
	currenttime = SDL_GetTicks();
	dt = currenttime - t_prev;
	t_prev=currenttime;
	
	//UPDATE PARTICLE SYSTEMS---------------------------
	updateParticleSystems(dustparticles, dt, camera);
	updateParticleSystems(healthparticles, dt, camera);
	//animate minimap indicators
	if(currenttime-lastframetime2>200){
		animate=1;
		lastframetime2 = currenttime;
	}
	else
		animate=0;
		
	///------------TIME SYSTEM------------	
	if (timer - oldTimer >= 1000) { // Check if a second has passed
		oldTimer = timer;
		game_time--;

		if (game_time >= 0) {
		    minutes = game_time / 60;
		    seconds = game_time % 60;

		    if (seconds < 10)
		        sprintf(timerTxt, "\n0%d:0%d", minutes, seconds);
		    else
		        sprintf(timerTxt, "\n0%d:%d", minutes, seconds);

		    if (game_time == 30) {
		        // Turn it into red(255,0,0) 
		        timerSurface.color.g = 0;
		        timerSurface.color.b = 0;
		    }
		    else if(timerSurface.color.g==0 && game_time>30){//if reward made game time go from under 30 to above 30, turns it white again
		    	timerSurface.color.g = 255;
		        timerSurface.color.b = 255;
		    }
		}
    }
    	//INTRO------------
    	if(intro_frames<4 && game_time_const - game_time >=4 && blackbartop.pos1.y >= -105 ){
    		
    		intro_frames++;
    		blackbartop.pos1.y-=15;
    		blackbarbottom.pos1.y+=15;	
    	}
    	currentAnimTime=SDL_GetTicks();
    	
    	/////----------PAUSE MENU FRAMES ANIMATION-------
    	if(pausemenuActive && currentAnimTime - lastFrameTime > 50){
    		if(hoveredPauseMenu!=-1){
    			for(i=0;i<4;i++){
    				if(i!=hoveredPauseMenu){
		    			if(pausemenuframes[i]>0)
						pausemenuframes[i]--;
					}
				else{
					if(pausemenuframes[i]<7)
						pausemenuframes[i]++;
					}
				}
				}
		else{
			for(i=0;i<4;i++){
				if(pausemenuframes[i]>0)
					pausemenuframes[i]--;
			}
		}
		
		if(pausemenubgframes<1)
			pausemenubgframes++;
    		lastFrameTime =  currentAnimTime;
    	}
    	//////////update cards frames  if selected or not --------MEMORY CARD GAME
    	if(memorygameactive){
	if(selectedcard1!=-1 && cardsframes[selectedcard1]<5)
		cardsframes[selectedcard1]++;
	else if(lastrightcard1!=previouscard1 && selectedcard1==-1 &&  cardsframes[previouscard1]>0)
		cardsframes[previouscard1]--;
		
	if(selectedcard2!=-1 && cardsframes[selectedcard2]<5)
		cardsframes[selectedcard2]++;
	else if(lastrightcard2!=previouscard2 && selectedcard2==-1 &&  cardsframes[previouscard2]>0)
		cardsframes[previouscard2]--;
		
		
		}
		
	//DEBUT INTEGRATION
	if(p1.hp<0){
		p1.hp=0;
		p1.laststatus = p1.status;
		p1.status = PLAYER_DEAD;	
	}
	p1.currentRow = currentPlayerAnimation(p1);
	if(!pausemenuActive && intro_frames>=4){

		handlePlayerKeyHeldEvent(&p1, keystate, dt);
		handlePlayerAnimation(&p1, currenttime, dt);
		updatePlayerHW(&p1, p1.clip[p1.currentRow][p1.frame]);
		//CollisionDetection(&bgmask, &p1, camera);
		//apply_gravity(&p1, dt); 
		int ca=  CollisionDetectionLeft(&bgmask, &p1, camera);
		int cb= CollisionDetectionRight(&bgmask, &p1, camera);
		int cc= CollisionDetectionBottom(&bgmask, &p1, camera);
		int cd= CollisionDetectionTop(&bgmask, &p1, camera);
		if((ca==2 || cb==2 || cc == 2 || cd==2) && currenttime - p1.lastdamaged> 1200){
			if(p1.hp> 0){
				p1.hp--;
				Mix_PlayChannel(2,health_hit_sfx, 0);
				p1.lastdamaged=currenttime;
			}
			if(p1.hp<=0)
				p1.status = PLAYER_DEAD;	
		}
		if(ca==3 || cb==3 || cc == 3 || cd==3){
			p1.hp=0;
			p1.status = PLAYER_DEAD;
				
		}
		if(p1.status != PLAYER_DEAD)											
			move_player(&p1, dt);
		printf("current clip width: %d\n", p1.clip[p1.currentRow][p1.frame].w);
		if(p1.status!=PLAYER_IDLE && !p1.isJumping &&  !p1.cameraIsScrolling){
			int modifier=10;
			if(p1.isRunning)
				modifier=1;
			if(p1.status==PLAYER_RIGHT)
				emitParticles(dustparticles, p1.pos.x+10, p1.pos.y+p1.playerH-5 , PLAYER_RIGHT, modifier);
			else
				emitParticles(dustparticles, p1.pos.x+p1.playerW-10, p1.pos.y+p1.playerH-5, PLAYER_LEFT, modifier);
			
			
		}
	}
	camera.directionX = 0;
	camera.directionY = 0;
	updateCameraVertical(&camera, &p1,  1920, 1080, bg.img->w, bg.img->h);
	printf("camera pos y: %d\n", camera.pos.y);
	if(!p1.rc && !p1.lc || p1.bc){
		/*if(p1.status==PLAYER_RIGHT){
			scrollHorizontal(&camera, 1, p1.lastdx) ;
			scrollVertical(&camera, 1, p1.lastdy);
		}	
		else if(p1.status==PLAYER_LEFT){
			scrollHorizontal(&camera, 2, p1.lastdx) ;
			scrollVertical(&camera, 1, p1.lastdy);
		}	*/
	}
	
	//printf("curent row: %d | player frame: %d\n", p1.currentRow, p1.frame);
	renderBackground(ecran, bg,&bgmask,  &camera, &p1) ;
	//updateObstaclesWithCamera(obstacles, 4, camera);
	printf("current row: %d | current frame: %d\n", p1.currentRow, p1.frame);
	/*if(abs(p1.pos.x - p1.prev_pos.x )> 500)
		p1.pos.x = p1.prev_pos.x;
	if(p1.pos.x > 1920 || p1.pos.x <0)
		p1.pos.x = p1.prev_pos.x;
	if(p1.pos.y+ camera.pos.y <0 || p1.pos.y + camera.pos.y > bg.img->h){
		printf("OUT OF BOUNDS");		
		p1.pos.y = 5000;
	}*/
	display_player(p1, ecran, playerspritesheets[p1.currentRow], &p1.clip[p1.currentRow][p1.frame]);
	updateMiniplayerVertical(&m, p1, camera);
	if(!gameover && !gamewon){
	for(int z=0;z<n_enemy;z++){
		int enemy_killed = 0;
		if(!tutorials.active){
		move_enemy(&e[z], p1, dt, camera);
		ai_enemy(&p1, &e[z], camera, currenttime);
		enemy_killed = check_enemy(&p1, &e[z], camera, health_hit_sfx);
		animer_enemy(&e[z],currenttime, dt);
		}
		afficher_enemy(e[z], ecran, camera);
		printf("\n-----------\nenemy %d | status: %d\n\n\n", z, e[z].status);
		if(enemy_killed==1)
			{
				update_objective(&obj[1], 1);//test  objective update
				Mix_PlayChannel(3,collect_sfx, 0);
			}
		
	}
	}
	
	//minimap	
	afficher_minimap(m, ecran);
	//----------------NPCCCCCCCCCCCC-------------------//Declare an array of multiple npc (related to save game)
	if(!gameover && !gamewon){
	for(int k=0;k<n_npc;k++){
		checkPlayerInProximity(&npcs[k], p1, camera);
		display_npc(npcs[k], ecran, camera);
		if(npcs[k].isActive && npcs[k].dialogueActive==0)//npc dialogue is finished and waiting for next step (if he has a job -> execute it : do nth)
		{
			printf("npc job = %d\n", npcs[k].job);
			int s=0;
			p1.isRunning=0;
			p1.acceleration = 5;
			p1.vitesse=0;
			p1.laststatus=p1.status;
			p1.status = PLAYER_IDLE;
			switch(npcs[k].job){
				case 0: break;
				case 1: s = startortcrushgame(ecran, 60);
					score += s;
					npcs[k].isActive=0;
					if(s!=0)
						npcs[k].job_completed=1;
					break;
				case 2:  s = starttic(ecran);
					score+=  s* random_int(700, 900);npcs[k].isActive=0;npcs[k].job_completed=1;break;
				case 3: s = enigme(ecran);
					score += s * random_int(100, 400);npcs[k].isActive=0;
					if(s!=0)
						npcs[k].job_completed=1;
					break;
				case 5: activate_doc(&documents);
					update_objective(&obj[2], 1);
					Mix_PlayChannel(3,collect_sfx, 0);
					npcs[k].isActive=0;
					npcs[k].job_completed=1;
					break;
				case 6: activate_tuto(&tutorials);
					npcs[k].isActive=0;
					npcs[k].job_completed=1;
					break;
				
				default:break;
			}
			if(npcs[k].job_completed && npcs[k].job != 0 && npcs[k].job!=5 && npcs[k].job !=6 && s!=0){
				int r;
				reward = rewardPlayer(ecran, cursorImage4,cur_pos, oldCurPosX,oldCurPosY, &r);
				SDL_Delay(200);
				switch(reward){
					case 1:
						printf("life added\n");
						if(p1.hp<4)
							p1.hp++;
						Mix_PlayChannel(2,heal_sfx, 0);//add condition to remove hp as a reward option iff full
						break;
					case 2:
						printf("time added\n");
						game_time +=r;
						Mix_PlayChannel(2,heal_sfx, 0);
						break;
					case 3:
						printf("resources added\n");
						update_objective(&obj[0], r);//test  objective update
						Mix_PlayChannel(3,collect_sfx, 0);
						break;
				
				}
			}
		
		}
	}
	}
	//npcOnInteraction(&npc1, ecran, event);
	if(p1.isJumping){
		jump(&p1, dt, p1.pos.x, p1.lastjumpY);
		if(p1.posRel.x <-60)
			emitParticlesExplosion(dustparticles,  p1.pos.x+p1.playerW*0.5, p1.pos.y+p1.playerH, 1, 3, 0.5);	
	}
	animate_minimap_indicator(&ind_enigma1, ecran, animate);
	//printf("jumping: %d | pos rel y: %d\n", p1.isJumping, p1.posRel.y);
	animate_minimap_indicator(&ind_enigma2, ecran, animate);
	//SCROLLING INDICATORS
	//SDL_FillRect(ecran, &(SDL_Rect){SCREEN_W/3, 0, 10, 1080}, SDL_MapRGB(ecran->format, 255, 255, 255));
	//SDL_FillRect(ecran, &(SDL_Rect){SCREEN_W/2, 0, 10, 1080}, SDL_MapRGB(ecran->format, 255, 255, 255));
	
	//game object
	if(!gameover && !gamewon){
	switch(update_gameObject(gameObjects, &n_gameObj, p1, camera)){//player interacted with the game obejct
		case -1: break;//player didnt interact with a game object
		case 1: p1.isRunning=0;
			p1.acceleration = 5;
			p1.vitesse=0;
			p1.laststatus=p1.status;
			p1.status = PLAYER_IDLE;
			score += startortcrushgame(ecran, 30);p1.pos.x -=p1.lastdx;break;//start ortcrush
		case 2: p1.isRunning=0;
			p1.acceleration = 5;
			p1.vitesse=0;
			p1.laststatus=p1.status;
			p1.status = PLAYER_IDLE;
			score += starttic(ecran) * random_int(500, 1000);p1.pos.x -= p1.lastdx;break;//tic return type: 0-lost 1->tie  2- won 
		case 3: p1.isRunning=0;
			p1.acceleration = 5;
			p1.vitesse=0;
			p1.laststatus=p1.status;
			p1.status = PLAYER_IDLE;
			score += enigme(ecran) *  random_int(100, 400);
			p1.pos.x -= p1.lastdx;
			printf("p1 pos after enigma: %d , %d \n\n\n", p1.pos.x, p1.pos.y);
			break;
		case 4: 
			update_objective(&obj[0], 1);//test  objective update
			Mix_PlayChannel(3,collect_sfx, 0);
			break;
		case 5: if(p1.hp<4){
				p1.hp++;
				Mix_PlayChannel(4,heal_sfx, 0);
			}
			//emitParticlesExplosion(healthparticles,  p1.pos.x+p1.playerW*0.5, p1.pos.y-10, 10, 2, 2);
			emitParticles(healthparticles, p1.pos.x+p1.playerW*0.5, p1.pos.y-10 , PLAYER_RIGHT, 1);
			break;//add 1hp
		case 6:
			game_time+= random_int(5, 10);break;//add time
		case 7: 
			p1.isRunning=0;
			p1.acceleration = 5;
			p1.vitesse=0;
			p1.laststatus=p1.status;
			p1.status = PLAYER_IDLE;
			activate_doc(&documents);
			update_objective(&obj[2], 1);
			Mix_PlayChannel(3,collect_sfx, 0);
			score+=random_int(8, 20);
			break;
		case 9:

			update_objective(&obj[0], 1);
			Mix_PlayChannel(3,collect_sfx, 0);
			score+=random_int(8, 20);
			break;
			
	}
	printf("ngameObj: %d \n", n_gameObj);
	}
	
	display_gameObjects(gameObjects, n_gameObj, camera,ecran, currenttime);
	
	
	
	//LOGS
	char pos1txt[40], pos2txt[40];
	sprintf(pos1txt, "Pos X: %d", p1.pos.x+camera.pos.x);
	sprintf(pos2txt, "Pos Y: %d", p1.pos.y+camera.pos.y-500);
	afficher_text(ecran, &info2, pos1txt);
	afficher_text(ecran, &info4, pos2txt);

		
	if(p1.bc)
		afficher_text(ecran, &info3, "BOTTOM COLLISION");
	else
		afficher_text(ecran, &info3, "NO BOTTOM COLLISION");
		
	/*if(p1.tc)
		afficher_text(ecran, &info4, "TOP COLLISION");
	else
		afficher_text(ecran, &info4, "NO TOP COLLISION");*/
	
	if(p1.rc)
		afficher_text(ecran, &info5, "RIGHT COLLISION");
	else
		afficher_text(ecran, &info5, "NO RIGHT COLLISION");
	
	if(p1.lc)
		afficher_text(ecran, &info6, "LEFT COLLISION");
	else
		afficher_text(ecran, &info6, "NO LEFT COLLISION");
		
	
		
	//FIN INTEGRATION nadhamhom mbaad
	
	if(intro_frames<4){
		afficher_sprite(intro_img_spritesheet, ecran, &intro[intro_frames]);
		afficher_image(blackbartop, ecran);
		afficher_image(blackbarbottom, ecran);	
	}
	else{
	
		//afficher_text(ecran, &txt_hit, "HIT");
		//afficher_text(ecran, &txt_returnmenu, "PLAY MEMORYCARD GAME");
		//afficher_text(ecran, &txt_collect, "COLLECT");
		//afficher_text(ecran, &txt_ortcrush, "PLAY ORTCRUSH");
		//afficher_text(ecran, &txt_enigme, "PLAY ENIGME");
		afficher_text(ecran, &txt_tictactoe, "DISPLAY TUTORIAL");
		printf("hp: %d\n",p1.hp);
		afficher_image(hb[4-p1.hp], ecran);
		afficher_image(healthicon, ecran);
		afficher_image(rightline, ecran);
		displayStaminaBar(p1, staminabar, ecran);
		afficher_image(timerbg, ecran);
		afficher_text(ecran, &timewordSurface, "Time left");
		sprintf(scoretotxt, "%d", score);
		afficher_text(ecran, &txt_playerScore, scoretotxt);//needs change or remove later
		afficher_text(ecran, &timerSurface, timerTxt);
		afficher_objective(obj[0], ecran);
		afficher_objective(obj[1], ecran);
		afficher_objective(obj[2], ecran);
		afficher_image(tutorial_btn, ecran);
		afficher_image(scroll_btn, ecran);
		//TUTORIAL DISPLAY
		display_tutorials(&tutorials, ecran);
		display_documents(&documents, ecran);
		display_doc(&documents, ecran);
	}
	//memory game ------------MAKE IT A FUNCTION-----------------
    	if(memorygameactive==1 && memorygamescore!=6){
    		SDL_FillRect(ecran, &(SDL_Rect){0, 0, 1920, 1080}, SDL_MapRGB(ecran->format, 155, 155, 155));
    		//afficher_image(blurredbg, ecran);
    		afficher_memorygame(memorygamerect, memorygame, ecran, cardsframes);
    		if(selectedcard1!=-1 && selectedcard2!=-1){
    			if(cardsframes[selectedcard2]==5){
	    		printf("selected card 1: %d , selected card 2 : %d\n", selectedcard1, selectedcard2);
	    		if(check_memorygame(cardsframes, memorygame_answers, selectedcard1, selectedcard2)==1){
	    			printf("\n right");
	    			memorygamescore+=2;
	    			memorygame_flipped[selectedcard1] = 1;
	    			memorygame_flipped[selectedcard2] = 1;//makes sure right cards arent flipped or cant be selected again
	    			previouscard1=selectedcard1;
	    			previouscard2=selectedcard2;
	    			lastrightcard1=selectedcard1;
	    			lastrightcard2=selectedcard2;
	    			selectedcard1=-1;
	    			selectedcard2=-1;
	    			
	    		}
	    		else{
	    			//SDL_Delay(2000);
	    			//if(previouscard1!=-1 && previouscard2 !=-1 && cardsframes[previouscard2]==0 &&  cardsframes[previouscard1]==0)
	    			printf("\n false");

	    			previouscard1=selectedcard1;
	    			previouscard2 = selectedcard2;
	    			selectedcard1=-1;
	    			selectedcard2=-1;
	    		}
	    		printf("memory game score: %d\n", memorygamescore);
    		}
    		}
    		if(memorygamescore==6){
    			//display reward ecran system
    			int r;
			reward = rewardPlayer(ecran, cursorImage4,cur_pos, oldCurPosX,oldCurPosY, &r);
			init_memorygame_answers(memorygame_answers);//testing
			 init_memorygame_card_frames(cardsframes);//init all cards frames to 0
			 memorygamescore=0;
			 for(int c=0;c<6;c++)
			 	memorygame_flipped[c]=0;
			SDL_Delay(200);
			switch(reward){
				case 1:
					printf("life added\n");
					if(p1.hp<4)
						p1.hp++;
					Mix_PlayChannel(2,heal_sfx, 0);//add condition to remove hp as a reward option iff full
					break;
				case 2:
					printf("time added\n");
					game_time +=r;
					Mix_PlayChannel(2,heal_sfx, 0);
					break;
				case 3:
					printf("resources added\n");
					update_objective(&obj[0], r);//test  objective update
					Mix_PlayChannel(3,collect_sfx, 0);
					break;
			
			}
			memorygameactive=0;
    		
    		}
    	}
	animate_objective(&obj[0], obj_hovered);
	animate_objective(&obj[1], obj_hovered2);
	animate_objective(&obj[2], obj_hovered3);
	//pause menu
	if(pausemenuActive){
		afficher_image(pausemenubg[pausemenubgframes], ecran);
		afficher_image(pausemenu, ecran);
		afficher_sprite(resume, ecran, &pausemenuframespos[pausemenuframes[0]]);
		afficher_sprite(restart, ecran, &pausemenuframespos[pausemenuframes[1]]);
		afficher_sprite(settings, ecran, &pausemenuframespos[pausemenuframes[2]]);
		afficher_sprite(exit, ecran, &pausemenuframespos[pausemenuframes[3]]);
		afficher_text(ecran, &txt_resume, "RESUME");
		afficher_text(ecran, &txt_restart, "RESTART");
		afficher_text(ecran, &txt_settings, "SETTINGS");
		afficher_text(ecran, &txt_exit, "EXIT");
		
		
	}
	
	//RENDER PARTICLES -----------------
	//add conditiosn to optimize this
	renderParticles(dustparticles, ecran, camera);//particles
	renderParticles(healthparticles, ecran, camera);//particles
	
	//-------WIN STATE----------
	//-------WIN STATE----------
	if(obj[0].done && obj[1].done && obj[2].done && p1.status != PLAYER_DEAD && !gameover){
		int d=-1;
		if(Mix_Playing(5))
			Mix_HaltChannel(5);
		if(!gamewon){//play once(dont mind the variable name)
			Mix_PlayChannel(4,gameover_sfx, 0);	
			gamewon=1;
		}
		p1.deathyes_txt.pos_txt.y = gamewon_img.pos1.y+SCREEN_H*0.7;
		p1.deathyes_txt.pos_txt.x = gamewon_img.pos1.y+SCREEN_W*0.43;
		p1.deathno_txt.pos_txt.y = gamewon_img.pos1.y+SCREEN_H*0.7;
		p1.deathscore_txt.pos_txt.y = gamewon_img.pos1.y+SCREEN_H*0.81;
		SDL_PollEvent(&event);
			switch(event.type){
			
				case SDL_MOUSEBUTTONDOWN:
					if(text_clicked(event, p1.deathyes_txt))
						d=1;
					else if(text_clicked(event, p1.deathno_txt))
						d=0;
					break;
				case SDL_MOUSEMOTION:
					if(text_hovered(event, p1.deathyes_txt)){
						p1.deathyes_txt.color.r = 182;
						p1.deathyes_txt.color.g = 163;
						p1.deathyes_txt.color.b = 109;
					}
					else
					{
						p1.deathyes_txt.color.r = 255;
						p1.deathyes_txt.color.g = 255;
						p1.deathyes_txt.color.b = 255;
					}
						
					if(text_hovered(event, p1.deathno_txt)){
						p1.deathno_txt.color.r = 182;
						p1.deathno_txt.color.g = 163;
						p1.deathno_txt.color.b = 109;
					}
					else
					{
						p1.deathno_txt.color.r = 255;
						p1.deathno_txt.color.g = 255;
						p1.deathno_txt.color.b = 255;
					}
						
					break;

			}
		
		
		afficher_image(gamewon_img, ecran);
		afficher_text(ecran, &p1.deathyes_txt, "Yes");
		afficher_text(ecran, &p1.deathno_txt, "No");
		afficher_text(ecran, &p1.deathscore_txt, scoretotxt);
		printf("d :%d\n", d);
		if(d==0){
			*page = 1;
			level_active=0;
		}
		else if(d==1){
			printf("back to main menu \n");
			remove("level1.dat");//remove file so the user can play the level again (completed)
			*page = 1;
			level_active=0;
		}
	
	}
	//---PLAYER DEATH SCREEN-------------
	if(((p1.status == PLAYER_DEAD && p1.frame >=4) || game_time<0) && !gamewon){
		int d=-1;
		if(Mix_Playing(5))
			Mix_HaltChannel(5);
		if(!gameover){//play once
			Mix_PlayChannel(4,gameover_sfx, 0);	
			gameover=1;
		}
		SDL_PollEvent(&event);
			switch(event.type){
			
				case SDL_MOUSEBUTTONDOWN:
					if(text_clicked(event, p1.deathyes_txt))
						d=1;
					else if(text_clicked(event, p1.deathno_txt))
						d=0;
					break;
				case SDL_MOUSEMOTION:
					if(text_hovered(event, p1.deathyes_txt)){
						p1.deathyes_txt.color.r = 182;
						p1.deathyes_txt.color.g = 163;
						p1.deathyes_txt.color.b = 109;
					}
					else
					{
						p1.deathyes_txt.color.r = 255;
						p1.deathyes_txt.color.g = 255;
						p1.deathyes_txt.color.b = 255;
					}
						
					if(text_hovered(event, p1.deathno_txt)){
						p1.deathno_txt.color.r = 182;
						p1.deathno_txt.color.g = 163;
						p1.deathno_txt.color.b = 109;
					}
					else
					{
						p1.deathno_txt.color.r = 255;
						p1.deathno_txt.color.g = 255;
						p1.deathno_txt.color.b = 255;
					}
						
					break;

			}
		
		
		afficher_image(p1.deathscreen, ecran);
		afficher_text(ecran, &p1.deathyes_txt, "Yes");
		afficher_text(ecran, &p1.deathno_txt, "No");
		afficher_text(ecran, &p1.deathscore_txt, scoretotxt);
		printf("d :%d\n", d);
		if(d==0){
			*page = 1;
			level_active=0;
		}
		else if(d==1){
			printf("restart \n");
			remove("level3.dat");//remove save file and restart from all init positions (test this more)
			*page = 4;
			level_active=0;
		}
	}
	
	
	displayCursor(cursorImage4,cur_pos, oldCurPosX,oldCurPosY,ecran);
	SDL_Flip(ecran);
	
	// Calculate the time elapsed since the last FPS update
	currentTick = SDL_GetTicks() ;
	//cap frame rate
	if(currentTick- startTicks <1000/FRAMES_PER_SECOND)
		SDL_Delay((1000/FRAMES_PER_SECOND) - (currentTick-startTicks));
	
	}
	
	
	//SAVE LEVEL DATA 
	level1data.level = 1;
	level1data.playerX = p1.pos.x;
	level1data.playerY= p1.pos.y;
	level1data.cameraX=  camera.pos.x;
	level1data.cameraY= camera.pos.y;
	printf("level1 x: %d | level1 y: %d\n",level1data.playerX,level1data.playerY); 
	level1data.playerHP = p1.hp;
	level1data.n_npc = n_npc;
	level1data.Tnpc[0] = npcs[0];
	level1data.Tnpc[1] = npcs[1];
	printf("HP: %d\n", p1.hp);
	saveLevelData(1, &level1data);
	
	//-----
	
	//liberer
	if (cursorImage4 != NULL) {
	    SDL_FreeSurface(cursorImage4);
	    cursorImage4 = NULL;
	} 
	else {
	    fprintf(stderr, "Error: cursorImage is NULL\n");
	}
	liberer_text(&txt_returnmenu);
	liberer_text(&txt_hit);
	liberer_text(&txt_collect);
	liberer_text(&timerSurface);
	liberer_image(gamewon_img);
	liberer_text(&timewordSurface);
	liberer_text(&txt_resume);
	liberer_chunk(gameover_sfx);
	liberer_text(&txt_restart);
	liberer_text(&txt_settings);
	liberer_text(&txt_exit);
	liberer_text(&txt_ortcrush);
	liberer_text(&txt_playerScore);
	liberer_text(&txt_enigme);
	liberer_text(&txt_tictactoe);
	liberer_image(timerbg);
	printf("MEMORY CLEARED\n");
	liberer_image(tutorial_btn);
	liberer_image(scroll_btn);
	liberer_image(hb[0]);
	liberer_image(hb[1]);
	liberer_image(hb[2]);
	liberer_image(hb[3]);
	liberer_image(hb[4]);
	liberer_image(blackbartop);
	liberer_image(blackbarbottom);
	liberer_image(rightline);
	liberer_image(staminabar);
	printf("MEMORY CLEARED\n");
	liberer_image(pausemenubg[0]);
	liberer_image(pausemenubg[1]);
	liberer_image(resume);
	liberer_image(pausemenu);
	liberer_image(restart);
	liberer_image(settings);
	liberer_image(exit);
	liberer_objective(obj[0]);
	liberer_objective(obj[1]);
	liberer_objective(obj[2]);
	liberer_music(level2theme);
	liberer_chunk(health_hit_sfx);
	liberer_chunk(cardflip_sfx);
	liberer_chunk(heal_sfx);
	liberer_chunk(collect_sfx);
	free(playerspritesheets);
	printf("MEMORY CLEARED\n");
	liberer_image(bg);
	liberer_text(&info);
	liberer_text(&info2);
	liberer_text(&info3);
	liberer_text(&info4);
	liberer_text(&info5);
	liberer_text(&info6);
	liberer_image(bgmask);
	liberer_player(p1);
	liberer_minimap(m);
	liberer_indicator(ind_enigma1);
	liberer_indicator(ind_enigma2);
	liberer_image(intro_img_spritesheet);
	freeParticleArray(dustparticles);
	freeParticleArray(healthparticles);
	liberer_image(dustparticle);
	liberer_image(healthparticle);
	liberer_image(mainplatform);
	free(obstacles);
	printf("MEMORY CLEARED\n");
	for(int x=0;x<n_enemy;x++)
		liberer_enemy(e[x]);
	printf("MEMORY CLEARED\n");
	for(int k=0;k<n_npc;k++){	
		liberer_npc(npcs[k]);
	}
	printf("MEMORY CLEARED\n");
	for(i=0;i<6;i++)
		liberer_image(memorygame[i]);
	printf("MEMORY CLEARED\n");
	liberer_gameObjects(gameObjects, n_gameObj);
	liberer_tutorials(&tutorials);
	liberer_documents(&documents);
	printf("MEMORY CLEARED\n");
	
	/*integration manette
	serialport_flush(fd);
    	serialport_close(fd);*/
	Mix_CloseAudio();
}
