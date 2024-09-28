#include <SDL/SDL.h>
#include <SDL/SDL_image.h> 
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "ortcrush.h"
#include "ortcrushmain.h"
#include "../fonctions.h"
#include "../constants.h"



int startortcrushgame(SDL_Surface *screen, int gametime) {
    // Initialize SDL
    
    //TTF_Init();
    // Initialize game variables
    Candy grid[COLUMN_COUNT][ROW_COUNT];
    SDL_Surface*gridcell =IMG_Load("Ortcrush/ortcrushgridcell.png"); 
    SDL_Surface*gridcellselected =IMG_Load("Ortcrush/ortcrushgridcellselected.png");
    Image ortcrushbg, starwhite1, starwhite2, starwhite3, stargold1, stargold2, stargold3;
    int selectedColumn = -1, selectedRow = -1;
    int secondColumn = -1, secondRow = -1;
    int matches, m;
    int quit = 0, score=0,maxScore=1, game_time=gametime;//leave maxscore to 1
    int minutes=game_time/60, seconds=game_time%60;
    Image particleT[11];
    Uint32 timer, oldTimer;
    SDL_Rect cur_pos;
    float oldCurPosX=0, oldCurPosY=0;
    SDL_Surface *cursorImage4 = init_cursor(&cur_pos);
    char scoretotext[10]="", timertotext[15]="00:44";
    int randbg = random_int(2, 3);
    char bgchar[28];
    switch(randbg){
    	case 2: strcpy(bgchar, "Ortcrush/ortcrushbg2.jpg");break;
    	case 3: strcpy(bgchar, "Ortcrush/ortcrushbg3.jpg");
    
    }
    init_image(&ortcrushbg, bgchar, 0, 0);
    init_image(&starwhite1, "Ortcrush/ortcrushstarwhite.png", ortcrushbg.pos1.x+ortcrushbg.pos1.w*0.1, ortcrushbg.pos1.y+ortcrushbg.pos1.h*0.49);
    init_image(&starwhite2, "Ortcrush/ortcrushstarwhite.png", ortcrushbg.pos1.x+ortcrushbg.pos1.w*0.17, ortcrushbg.pos1.y+ortcrushbg.pos1.h*0.49);
    init_image(&starwhite3, "Ortcrush/ortcrushstarwhite.png", ortcrushbg.pos1.x+ortcrushbg.pos1.w*0.24, ortcrushbg.pos1.y+ortcrushbg.pos1.h*0.49);
    init_image(&stargold1, "Ortcrush/ortcrushstaryellow.png", ortcrushbg.pos1.x+ortcrushbg.pos1.w*0.09, ortcrushbg.pos1.y+ortcrushbg.pos1.h*0.49);
    init_image(&stargold2, "Ortcrush/ortcrushstaryellow.png", ortcrushbg.pos1.x+ortcrushbg.pos1.w*0.16, ortcrushbg.pos1.y+ortcrushbg.pos1.h*0.49);
    init_image(&stargold3, "Ortcrush/ortcrushstaryellow.png", ortcrushbg.pos1.x+ortcrushbg.pos1.w*0.23, ortcrushbg.pos1.y+ortcrushbg.pos1.h*0.49);
    Text scoretxt = init_text(ortcrushbg.pos1.x+ortcrushbg.pos1.w*0.16, ortcrushbg.pos1.y+ortcrushbg.pos1.h*0.71,"Ortcrush/BerlinSansFBRegular.ttf", 70, 255,255,255); 
    Text timertxt = init_text(ortcrushbg.pos1.x+ortcrushbg.pos1.w*0.1, ortcrushbg.pos1.y+ortcrushbg.pos1.h*0.28,"Ortcrush/BerlinSansFBRegular.ttf", 150, 255,255,255);
    Text scorePopUp = init_text(0,0,"Ortcrush/BerlinSansFBRegular.ttf", 30, 255,255,255); 
    
    //init particle fx array
    initParticleFX(particleT);
    //init progress bar
    SDL_Rect progressBarRect, progressBarScore;
    float totalProgress=0, currentProgress = 0;
    int star1active=2480, star2active=7080, star3active=11610;
    // Initialize the game grid
    //initializeGrid(grid);
    
    do{
    	initializeGrid(grid);
    	m = checkMatches(grid, &score,maxScore, &totalProgress, screen, particleT, scorePopUp, gridcell, gridcellselected, ortcrushbg, starwhite1, starwhite2, starwhite3,  timertxt,  scoretxt,  timertotext, scoretotext,  stargold1,  stargold2, stargold3,progressBarRect,  &progressBarScore,&currentProgress);
    	if(m>0){
    		freeGrid(grid);
    		printf("freed grid | matches=%d\n", m);	
    	}
    }while(m>0);
    
    printf("grid initialized\n");
    //reinitialize
    score=0;
    totalProgress=0;
    maxScore=15000;
    //reinitialization complete
    progressBarRect.x = ortcrushbg.pos1.x+ortcrushbg.pos1.w*0.07;
    progressBarRect.y = ortcrushbg.pos1.y+ortcrushbg.pos1.h*0.57;
    progressBarRect.w = SCREEN_W/4.3;
    progressBarRect.h = 30;
    progressBarScore.x = ortcrushbg.pos1.x+ortcrushbg.pos1.w*0.07;
    progressBarScore.y = ortcrushbg.pos1.y+ortcrushbg.pos1.h*0.57;
    progressBarScore.w = SCREEN_W/4.3*currentProgress;
    progressBarScore.h = 30;

    // Game loop
    while (!quit) {
        // Handle events
        timer = SDL_GetTicks();
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            } 
            else if(event.type==SDL_KEYDOWN){//to delete when integrating
            	if(event.key.keysym.sym==SDLK_f)
            		SDL_WM_ToggleFullScreen(screen);
            		
            		
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (handleInput(event,grid, &selectedColumn, &selectedRow, &secondColumn, &secondRow)) {
                    if (selectedColumn != -1 && selectedRow != -1 && secondColumn != -1 && secondRow != -1) {
                        // Swap selected candies (inside render grid func)
                        //&&
                        // Render the game grid with animation
                        printf("selected column 1: %d | selected row 1: %d ||||| selected column 2: %d | selected row 2: %d \n",selectedColumn, selectedRow, secondColumn, secondRow);
                        renderGrid2(screen, grid, selectedColumn, selectedRow, secondColumn, secondRow, gridcell, gridcellselected, ortcrushbg, starwhite1, starwhite2, starwhite3, timertxt, scoretxt, timertotext, scoretotext, stargold1, stargold2, stargold3, score,maxScore, totalProgress,progressBarRect,progressBarScore, currentProgress);
                        
                        // Check for matches
                        matches = checkMatches(grid, &score,maxScore, &totalProgress, screen, particleT, scorePopUp, gridcell, gridcellselected, ortcrushbg, starwhite1, starwhite2, starwhite3,  timertxt,  scoretxt,  timertotext, scoretotext,  stargold1,  stargold2, stargold3, progressBarRect,  &progressBarScore,&currentProgress);
                        printf("matches: %d\n", matches);
                        if(matches==0){
                        	printf("no matches\n");
                        	//swap back
                        	renderGrid2(screen, grid, selectedColumn, selectedRow, secondColumn, secondRow, gridcell, gridcellselected, ortcrushbg, starwhite1, starwhite2, starwhite3, timertxt, scoretxt, timertotext, scoretotext, stargold1, stargold2, stargold3, score,maxScore, totalProgress,progressBarRect,progressBarScore, currentProgress);
                        }
                        else{
//		  	        totalProgress+=0.4;
				displayCursor(cursorImage4,cur_pos, oldCurPosX,oldCurPosY,screen);//display cur
		                shiftCandies(grid, gridcell, gridcellselected, screen, ortcrushbg, starwhite1, starwhite2, starwhite3, timertxt, scoretxt, timertotext, scoretotext, stargold1, stargold2, stargold3, score,maxScore, totalProgress,progressBarRect,progressBarScore, currentProgress);
		  	        fillEmptyCol(grid, gridcell, gridcellselected, screen, ortcrushbg, starwhite1, starwhite2, starwhite3, timertxt, scoretxt, timertotext, scoretotext, stargold1, stargold2, stargold3, score,maxScore, totalProgress,progressBarRect,progressBarScore, currentProgress);       
			}
			while(checkMatches(grid, &score,maxScore, &totalProgress, screen, particleT, scorePopUp, gridcell, gridcellselected, ortcrushbg, starwhite1, starwhite2, starwhite3,  timertxt,  scoretxt,  timertotext, scoretotext,  stargold1,  stargold2, stargold3,progressBarRect,  &progressBarScore,&currentProgress)>0){  //if new candies are filled check matches until no candies are matching
				displayCursor(cursorImage4,cur_pos, oldCurPosX,oldCurPosY,screen);//display cur
				shiftCandies(grid, gridcell, gridcellselected, screen, ortcrushbg, starwhite1, starwhite2, starwhite3, timertxt, scoretxt, timertotext, scoretotext, stargold1, stargold2, stargold3, score,maxScore, totalProgress,progressBarRect,progressBarScore, currentProgress);
		  	        fillEmptyCol(grid, gridcell, gridcellselected, screen, ortcrushbg, starwhite1, starwhite2, starwhite3, timertxt, scoretxt, timertotext, scoretotext, stargold1, stargold2, stargold3, score,maxScore, totalProgress,progressBarRect,progressBarScore, currentProgress );
		  	        }     
		               
                        afficherGrid(grid);
                        /*if (matches > 0) {
                            // Process matches
                            int matchPositions[matches][2];
                            clearMatches(grid, matchPositions);
                            shiftCandies(grid);
                            fillTopRow(grid);
                            
                            // Additional logic after clearing matches, such as scoring, level progression, etc.
                            // ...
                        }
                        */
                        // Reset selected candies
                        selectedColumn = selectedRow = secondColumn = secondRow = -1;
                    }
                }
                }
                else if(event.type==SDL_MOUSEMOTION){
                	updateCursorPosition(&oldCurPosX, &oldCurPosY, &cur_pos, event);
                	
                }
        }
        
        //convert timer and score from int to text
        if (timer - oldTimer >= 1000) { // Check if a second has passed
		oldTimer = timer;
		game_time--;

		if (game_time >= 0) {
		    minutes = game_time / 60;
		    seconds = game_time % 60;

		    if (seconds < 10)
		        sprintf(timertotext, "0%d:0%d", minutes, seconds);
		    else
		        sprintf(timertotext, "0%d:%d", minutes, seconds);

		    if (game_time == 30) {
		        // Turn it into red(255,0,0) 
		        timertxt.color.g = 0;
		        timertxt.color.b = 0;
		    }
		}
    }
    
    	sprintf(scoretotext, "%d", score);
        //printf("time: %s\n", timertotext);
        afficher_image(ortcrushbg, screen); // Clear the screen
        //stars and condition (gold/white)
        if(score>=star1active){
        	afficher_image(stargold1, screen);
        }
        else{
        afficher_image(starwhite1, screen);   
        }
        if(score>=star2active){
        	afficher_image(stargold2, screen);
        }
        else{
        afficher_image(starwhite2, screen);   
        }
        if(score>=star3active){
        	afficher_image(stargold3, screen);
        }
        else{
        afficher_image(starwhite3, screen);   
        }
        afficher_text(screen, &timertxt, timertotext);
       
        afficher_text(screen, &scoretxt, scoretotext);
        // Render the progress bar
        renderGrid2(screen, grid, selectedColumn, selectedRow, secondColumn, secondRow, gridcell, gridcellselected, ortcrushbg, starwhite1, starwhite2, starwhite3, timertxt, scoretxt, timertotext, scoretotext, stargold1, stargold2, stargold3, score,maxScore, totalProgress,progressBarRect,progressBarScore, currentProgress);
        SDL_FillRect(screen, &progressBarRect, SDL_MapRGB(screen->format, 255, 255, 255));
    	while(currentProgress<totalProgress){
		currentProgress += (float) totalProgress / 6;
		printf("current progress= %f | total prog= %f | score: %d\n",currentProgress, totalProgress, score);
		if (currentProgress > totalProgress) {
			currentProgress = totalProgress;
		}
		progressBarScore.x = ortcrushbg.pos1.x+ortcrushbg.pos1.w*0.07;
		progressBarScore.y = ortcrushbg.pos1.y+ortcrushbg.pos1.h*0.57;
		progressBarScore.w = currentProgress*(SCREEN_W/4.3);  	
		progressBarScore.h = 30;
	    	SDL_FillRect(screen, &progressBarScore, SDL_MapRGB(screen->format, 194, 255, 210));
	    	SDL_Flip(screen); 
    	}
    	SDL_FillRect(screen, &progressBarScore, SDL_MapRGB(screen->format, 194, 255, 210));
       //check game ended or not
       if(game_time<=0){
       		quit=1;
       }
        displayCursor(cursorImage4,cur_pos, oldCurPosX,oldCurPosY,screen);
        SDL_Flip(screen); 
        
        
    }

    // Quit SDL
    if (cursorImage4 != NULL) {
	    SDL_FreeSurface(cursorImage4);
	    cursorImage4 = NULL;
	} 
	else {
	    fprintf(stderr, "Error: cursorImage is NULL\n");
	}
    freeGrid(grid);
    SDL_FreeSurface(gridcell);
    SDL_FreeSurface(gridcellselected);
    liberer_image(ortcrushbg);
    liberer_image(starwhite1);
    liberer_image(starwhite2);
    liberer_image(starwhite3);
    liberer_image(stargold1);
    liberer_image(stargold2);
    liberer_image(stargold3);
    liberer_text(&timertxt);
    liberer_text(&scoretxt);
    liberer_text(&scorePopUp);
    return score;
}








