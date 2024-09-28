#include <SDL/SDL.h>
#include <SDL/SDL_image.h> 
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "ortcrush.h"
#include <stdlib.h>
#include <time.h>
#include "../fonctions.h"
#include "../constants.h"
#define MIN_MATCHES 3 // min number of candies for a match
#define SHIFT_ANIMATION_DURATION 100 // Animation duration mtaa shifting candies
#define SHIFT_ANIMATION_SPEED 30 // animation speed mtaa shifting candies
#define ANIMATION_DURATION 150 // swapping duration 
const int GRID_X= SCREEN_WIDTH*0.5;
const int GRID_Y= SCREEN_HEIGHT*0.15;


void renderGrid2(SDL_Surface* screen, Candy grid[COLUMN_COUNT][ROW_COUNT], int selectedCol1, int selectedRow1, int selectedCol2, int selectedRow2, SDL_Surface *gridcell, SDL_Surface *gridcellselected, Image ortcrushbg, Image starwhite1,  Image starwhite2, Image starwhite3, Text timertxt, Text scoretxt, char timertotext[], char scoretotext[], Image stargold1, Image stargold2, Image stargold3,int score, int maxScore,float totalProgress,SDL_Rect progressBarRect, SDL_Rect progressBarScore,float currentProgress ) //swaps two candies if two are selected else, display it as it is
{
    // If two candies are selected for swapping, perform animation
    if (selectedCol1 != -1 && selectedRow1 != -1 && selectedCol2 != -1 && selectedRow2 != -1) {
        // Initialize variables for animation
        Uint32 startTime = SDL_GetTicks();
        int elapsedTime = 0;
        int startX1 = selectedCol1 * 100;
        int startY1 = selectedRow1 * 100;
        int startX2 = selectedCol2 * 100;
        int startY2 = selectedRow2 * 100;
        int endX1 = startX2;
        int endY1 = startY2;
        int endX2 = startX1;
        int endY2 = startY1;

	   int star1active=2480, star2active=7080, star3active=11610;//change gold star conditions here as well as ortcrushmain.c
        while (elapsedTime < ANIMATION_DURATION) {
            float progress = (float)elapsedTime / ANIMATION_DURATION;

            // current positions based on the trajectory between the two candies
            int currentX1 = startX1 + (endX1 - startX1) * progress;
            int currentY1 = startY1 + (endY1 - startY1) * progress;
            int currentX2 = startX2 + (endX2 - startX2) * progress;
            int currentY2 = startY2 + (endY2 - startY2) * progress;
	   afficher_image(ortcrushbg, screen); // Clear the screen
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
           SDL_FillRect(screen, &progressBarRect, SDL_MapRGB(screen->format, 255, 255, 255));
	   while(currentProgress<totalProgress){
			currentProgress += (float) totalProgress / 6;
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
            // Render the game grid except for the currently animating selected candies
            for (int col = 0; col < COLUMN_COUNT; col++) {
                for (int row = 0; row < ROW_COUNT; row++) {
		        SDL_Rect destRect = {col * 100 + GRID_X, row * 100+ GRID_Y, 100, 100};
		        if(grid[col][row].type == -1)
		        	SDL_BlitSurface(gridcell, NULL, screen, &destRect);
		        else if(col==selectedCol2 && row==selectedRow2 || col==selectedCol1 && row==selectedRow1){
        			SDL_BlitSurface(gridcellselected, NULL, screen, &destRect);
			}
                        if(grid[col][row].type != -1 && ((col != selectedCol1 || row != selectedRow1) &&  (col != selectedCol2 || row != selectedRow2) )){
                	        SDL_BlitSurface(gridcell, NULL, screen, &destRect);
		                SDL_BlitSurface(grid[col][row].image, NULL, screen, &destRect);
                        }
                    }
            }
            // render selected candies at their current positions
            SDL_Rect destRect1 = {currentX1 + GRID_X, currentY1+GRID_Y, 100, 100};
            SDL_BlitSurface(grid[selectedCol1][selectedRow1].image, NULL, screen, &destRect1);
            SDL_Rect destRect2 = {currentX2 + GRID_X, currentY2+ GRID_Y, 100, 100};
            SDL_BlitSurface(grid[selectedCol2][selectedRow2].image, NULL, screen, &destRect2);
            while(currentProgress<totalProgress){
			currentProgress += (float) totalProgress / 6;
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

            SDL_Flip(screen); 

            // change this
            SDL_Delay(1000 / 60);

            // Update elapsed time
            elapsedTime = SDL_GetTicks() - startTime;
        }

        // swap candies 
        swapSelectedCandies(grid, selectedCol1, selectedRow1, selectedCol2, selectedRow2);
    }
    else{
    // rendri l candies marokhra
    
    int col, row, j;
    for (col = 0; col < COLUMN_COUNT; col++) {
        for (row = 0; row < ROW_COUNT; row++) {
                SDL_Rect destRect = {col * 100 + GRID_X, row * 100 + GRID_Y, 100, 100};//change screen height and width proportions

        	
                
        	if(col==selectedCol1 && row==selectedRow1){
        		SDL_BlitSurface(gridcellselected, NULL, screen, &destRect);
        	}
        	else if(col==selectedCol2 && row==selectedRow2){
        		SDL_BlitSurface(gridcellselected, NULL, screen, &destRect);
		}
        	else
        		SDL_BlitSurface(gridcell, NULL, screen, &destRect);
                if(grid[col][row].type!=-1){
                	SDL_BlitSurface(grid[col][row].image, NULL, screen, &destRect);
               	}
    	}
}
    }
}


void initializeGrid(Candy grid[COLUMN_COUNT][ROW_COUNT]) {
    // Initialize the grid with random candy types
    int newType, lastType;
    srand(time(NULL));
    printf("row count: %d | col count: %d", ROW_COUNT, COLUMN_COUNT);
    for (int col = 0; col < COLUMN_COUNT; col++) {
        for (int row = 0; row < ROW_COUNT; row++) {
            newType = getRandomInt(0, 5);
            //probability system
            if ((col>1 &&newType ==grid[col-1][row].type)&& rand() % 2== 0) {
                // If the new type is the same as the last one, with a 50% probability, change it
                newType = (newType + 1) % 5;
            }
            if ((row>1 && newType ==grid[col][row-1].type)  && rand() % 2== 0) {
                // If the new type is the same as the last one, with a 50% probability, change it
                newType = (newType + 1) % 5;
            }
            if(row<ROW_COUNT-2 && newType ==grid[col][row+1].type)
            {
                // If the new type is the same as the last one, with a 50% probability, change it
                newType = (newType + 1) % 5;
            }
            if(col<COLUMN_COUNT-2 && newType ==grid[col+1][row].type)
            {
                // If the new type is the same as the last one, with a 50% probability, change it
                newType = (newType + 1) % 5;
            }
            grid[col][row].type = newType;
            printf("row: %d | column: %d | type: %d\n", row, col, grid[col][row].type);
            switch(grid[col][row].type){
            	case 0: grid[col][row].image = IMG_Load("Ortcrush/book1ortcrush.png");break;
            	case 1: grid[col][row].image = IMG_Load("Ortcrush/book2ortcrush.png");break;
            	case 2: grid[col][row].image = IMG_Load("Ortcrush/book3ortcrush.png");break;
            	case 3: grid[col][row].image = IMG_Load("Ortcrush/book4ortcrush.png");break;
            	case 4: grid[col][row].image = IMG_Load("Ortcrush/book5ortcrush.png");break;
            	case 5: grid[col][row].image = IMG_Load("Ortcrush/book6ortcrush.png");break;
            }
        }
    }
}


int checkMatches(Candy grid[COLUMN_COUNT][ROW_COUNT], int *score,int maxScore, float *totalProgress, SDL_Surface *screen, Image particleT[] ,Text scorePopUp,SDL_Surface *gridcell, SDL_Surface *gridcellselected, Image ortcrushbg, Image starwhite1,  Image starwhite2, Image starwhite3, Text timertxt, Text scoretxt, char timertotext[], char scoretotext[], Image stargold1, Image stargold2, Image stargold3,SDL_Rect progressBarRect, SDL_Rect *progressBarScore,float *currentProgress) {
    int matches = 0, j, consec, scoreadd=0;

    // Check for horizontal matches
  for (int row = 0; row < ROW_COUNT; row++) {
        for (int col = 0; col < COLUMN_COUNT; col++) {
            int type = grid[col][row].type;
            if(type!=-1){
            j=col;
            consec=0;
            while(j<COLUMN_COUNT && grid[j][row].type!=-1 && type==grid[j][row].type){
            	j++;
            	consec++;}
           
            if(consec>=3){
            	 printf("consec=%d | type=%d | row=%d |col=%d to%d\n", consec, type, row, col, j); 
            	 matches++;
            	 if(maxScore!=1){//prevent it from happening during grid init
            	 switch(type){
            	 	case 0: scoreadd=100;break;
            	 	case 1: scoreadd=150;break;
            	 	case 2: scoreadd=250;break;
            	 	case 3: scoreadd=300;break;
            	 	case 4: scoreadd=325;break;
            	 	case 5: scoreadd=350;break;
            	 }
            	 *score+=scoreadd*consec;
            	 //text score animation (+scoretotaladditionby type)
            	 //candy lightning ray animation
            	 printf("score: %d\n", *score);
		if(*totalProgress<1.0)            		
    			*totalProgress=(float)*score/maxScore;
            	 displayParticleFXAnimationHorizontal(screen, particleT, row, col, j, scorePopUp, scoreadd, gridcell, gridcellselected, ortcrushbg, starwhite1, starwhite2, starwhite3,  timertxt,  scoretxt,  timertotext, scoretotext,  stargold1,  stargold2, stargold3, *score,  maxScore, *totalProgress,progressBarRect,  progressBarScore,currentProgress, grid);
            	 
            	for(int x=col; x<j;x++){
            		
            		grid[x][row].type=-1;
            	}
            	}
    		}
            }
            }
         }



    // Check for vertical matches
for (int col = 0; col < COLUMN_COUNT; col++) {
    for (int row = 0; row < ROW_COUNT; row++) {
        int type = grid[col][row].type;
        if (type != -1) {
            j = row;
            consec = 0;
            while (j < ROW_COUNT && grid[col][j].type != -1 && type == grid[col][j].type) {
                j++;
                consec++;
            }
		
            if (consec >= 3) {
                printf("Vertical match: consec=%d | type=%d | col=%d | row=%d to %d\n", consec, type, col, row, j);
                matches++;
                if(maxScore!=1){//prevent it from happening during grid init
                switch(type){
            	 	case 0: scoreadd=100;break;
            	 	case 1: scoreadd=150;break;
            	 	case 2: scoreadd=250;break;
            	 	case 3: scoreadd=300;break;
            	 	case 4: scoreadd=325;break;
            	 	case 5: scoreadd=350;break;
            	 }
            	 *score+=scoreadd*consec;
            	 displayParticleFXAnimationVertical(screen, particleT, col, row, j, scorePopUp, scoreadd, gridcell, gridcellselected, ortcrushbg, starwhite1, starwhite2, starwhite3,  timertxt,  scoretxt,  timertotext, scoretotext,  stargold1,  stargold2, stargold3, *score,  maxScore, *totalProgress,progressBarRect,  *progressBarScore,*currentProgress, grid);
		if(*totalProgress<1.0)            		
    			*totalProgress=(float)*score/maxScore;
    		printf("score: %d\n", *score);
                for (int y = row; y < j; y++) {
                    grid[col][y].type = -1;
                }
                }
    		}
            }
        }
    }


    
    return matches;
}




void shiftCandies(Candy grid[COLUMN_COUNT][ROW_COUNT], SDL_Surface *gridcell, SDL_Surface *gridcellselected, SDL_Surface *screen, Image ortcrushbg, Image starwhite1,  Image starwhite2, Image starwhite3, Text timertxt, Text scoretxt , char timertotext[], char scoretotext[], Image stargold1, Image stargold2, Image stargold3,int score,int maxScore,float totalProgress,SDL_Rect progressBarRect, SDL_Rect progressBarScore,float currentProgress ) {
    // Shift candies down after clearing matches
    for (int col = 0; col < COLUMN_COUNT; col++) {
        for (int row = ROW_COUNT - 1; row >= 0; row--) {
            if (grid[col][row].type == -1) {
                // Find the first non-empty cell above the current one
                int aboveRow = row - 1;
                while (aboveRow >= 0 && grid[col][aboveRow].type == -1) {
                    aboveRow--;
                }
                if (aboveRow >= 0) {
                    // animate then Swap candies
                    animateFallingCandies(grid, col, row, aboveRow, gridcell, gridcellselected, screen, ortcrushbg, starwhite1, starwhite2, starwhite3, timertxt, scoretxt ,timertotext, scoretotext, stargold1, stargold2, stargold3, score,maxScore, totalProgress,progressBarRect,progressBarScore, currentProgress);
                    swapSelectedCandies(grid, col, row, col, aboveRow);
                    printf("now filling empty cols\n");
                }
            }
        }
    }
}


void animateFallingCandies(Candy grid[COLUMN_COUNT][ROW_COUNT], int colX, int endRow, int startRow, SDL_Surface *gridcell, SDL_Surface *gridcellselected, SDL_Surface *screen, Image ortcrushbg, Image starwhite1,  Image starwhite2, Image starwhite3, Text timertxt, Text scoretxt, char timertotext[], char scoretotext[], Image stargold1, Image stargold2, Image stargold3,int score, int maxScore,float totalProgress,SDL_Rect progressBarRect, SDL_Rect progressBarScore,float currentProgress ){
	// Initialize variables for animation
	//idea: if i want to make all candies fall at the same time, pass an array of columns with empty rows, and display their fall at the same time
        Uint32 startTime = SDL_GetTicks();
        int elapsedTime = 0;
        int X = colX * 100;
        int startY = startRow * 100;
        int endY = endRow * 100;

       int star1active=2480, star2active=7080, star3active=11610;//change gold star conditions here as well as main.c
        // Render the candies on the screen with animation
        while (elapsedTime < ANIMATION_DURATION) {
            // Calculate progress of animation
            float progress = (float)elapsedTime / ANIMATION_DURATION;

            // Calculate current positions based on the trajectory between the two candies
            int currentY = startY + (endY - startY) * progress;

            // Render the game grid except for the currently animating selected candies
             // Clear the screen / //CHANGE THIS TO DISPLAY BG AND ALL OTHER BG COMPONENTS
             afficher_image(ortcrushbg, screen); // Clear the screen
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
       
            for (int col = 0; col < COLUMN_COUNT; col++) {
                for (int row = 0; row < ROW_COUNT; row++) {
                        // Blit candy image onto the screen
		        SDL_Rect destRect = {col * 100+GRID_X, row * 100+GRID_Y, 100, 100};
        	        SDL_BlitSurface(gridcell, NULL, screen, &destRect);
                        if( grid[col][row].type != -1 ){
                        	if((col != colX || row!=startRow))
		                	SDL_BlitSurface(grid[col][row].image, NULL, screen, &destRect);
                        }
                    }
            }
            // Render selected candies at their current positions
            SDL_Rect destRect1 = {X+GRID_X, currentY+GRID_Y, 100, 100};
            SDL_BlitSurface(grid[colX][startRow].image, NULL, screen, &destRect1);
	    SDL_FillRect(screen, &progressBarRect, SDL_MapRGB(screen->format, 255, 255, 255));
	    SDL_FillRect(screen, &progressBarScore, SDL_MapRGB(screen->format, 194, 255, 210));
            SDL_Flip(screen); // Update the screen

            // Delay to control frame rate
            SDL_Delay(1000 / 60);

            // Update elapsed time
            elapsedTime = SDL_GetTicks() - startTime;
        }
}


void fillEmptyCol(Candy grid[COLUMN_COUNT][ROW_COUNT], SDL_Surface *gridcell, SDL_Surface *gridcellselected, SDL_Surface *screen, Image ortcrushbg, Image starwhite1,  Image starwhite2, Image starwhite3, Text timertxt, Text scoretxt, char timertotext[], char scoretotext[], Image stargold1, Image stargold2, Image stargold3,int score, int maxScore,float totalProgress,SDL_Rect progressBarRect, SDL_Rect progressBarScore,float currentProgress )
{
    int col, row ,j, newType;
    // init new candy then animate then Swap 
    for(col=0;col<COLUMN_COUNT;col++){
    
	for(row=ROW_COUNT-1; row>=0;row--){
	    if(grid[col][row].type==-1){
		    //for(j=row-1;j>0;j--){
			    newType = getRandomInt(0, 5);
			    //probability system
			    if ((col>1 &&newType ==grid[col-1][row].type)) {
				// If the new type is the same as the last one, with a 10% probability, change it
				newType = (newType + 1) % 5;
			    }
			    if ((col<COLUMN_COUNT-2  && newType ==grid[col+1][row].type)) {
				// If the new type is the same as the last one, with a 10% probability, change it
				newType = (newType + 1) % 5;
			    }
			    if ((row<ROW_COUNT-2 && newType ==grid[col][row+1].type)) {
				// If the new type is the same as the last one, with a 10% probability, change it
				newType = (newType + 1) % 5;
			    }
			    if ((row>1  && newType ==grid[col][row-1].type)) {
				// If the new type is the same as the last one, with a 10% probability, change it
				newType = (newType + 1) % 5;
			    }
			    grid[col][row].type = newType;
			    printf("new type: %d | column: %d\n", newType, col);
			    switch(grid[col][row].type){
			    	case 0: grid[col][row].image = IMG_Load("Ortcrush/book1ortcrush.png");break;
			    	case 1: grid[col][row].image = IMG_Load("Ortcrush/book2ortcrush.png");break;
			    	case 2: grid[col][row].image = IMG_Load("Ortcrush/book3ortcrush.png");break;
			    	case 3: grid[col][row].image = IMG_Load("Ortcrush/book4ortcrush.png");break;
			    	case 4: grid[col][row].image = IMG_Load("Ortcrush/book5ortcrush.png");break;
			    	case 5: grid[col][row].image = IMG_Load("Ortcrush/book6ortcrush.png");break;
			    }
			    if(row!=0)
			    	animateFallingCandies(grid, col, row, 0, gridcell, gridcellselected, screen, ortcrushbg, starwhite1, starwhite2, starwhite3, timertxt, scoretxt,timertotext,scoretotext, stargold1, stargold2, stargold3, score,maxScore, totalProgress,progressBarRect,progressBarScore, currentProgress);
		   // }
	    }
	}
}

}


int handleInput(SDL_Event event, Candy grid[COLUMN_COUNT][ROW_COUNT], int* selectedColumn, int* selectedRow, int* secondColumn, int* secondRow) {
    // Handle mouse clicks to select candies for swapping
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int x = (event.button.x - GRID_X) / 100;
        int y = (event.button.y - GRID_Y) / 100;
        printf("x: %d|y:%d\n",x, y); 
        if (x >= 0 && x < COLUMN_COUNT && y >= 0 && y < ROW_COUNT && grid[x][y].type != -1) {
            if (*selectedColumn == -1 && *selectedRow == -1) {
                // First candy selected
                *selectedColumn = x;
                *selectedRow = y;
            } else {
                // Second candy selected
                *secondColumn = x;
                *secondRow = y;
                
                // Check if the selected candies are adjacent
                if (abs(*selectedColumn - *secondColumn) + abs(*selectedRow - *secondRow) == 1) {
                    return 1; // Indicate that both candies are selected and adjacent
                } else {
                    // Clear the second selection
                    *secondColumn = -1;
                    *secondRow = -1;
                }
            }
        }
    }
    return 0; // Indicate that selection is incomplete
}

void swapSelectedCandies(Candy grid[COLUMN_COUNT][ROW_COUNT], int selectedColumn, int selectedRow, int newColumn, int newRow) {
    // Swap the selected candies in the grid
    Candy temp = grid[selectedColumn][selectedRow];
    grid[selectedColumn][selectedRow] = grid[newColumn][newRow];
    grid[newColumn][newRow] = temp;
}


void afficherGrid(Candy grid[COLUMN_COUNT][ROW_COUNT]){
	int col, row;
for (row = 0; row < ROW_COUNT; row++) {
    	for (col = 0; col < COLUMN_COUNT; col++) {
		    printf("%d | ", grid[col][row].type);
		}
		printf("\n");
	    }
}

int getRandomInt(int min, int max) {
    // Generate a random number between min and max(inclusive)
    return (rand() % (max - min + 1)) + min;
}

void freeGrid(Candy grid[COLUMN_COUNT][ROW_COUNT]){
	int col, row;
	for (row = 0; row < ROW_COUNT; row++) {
	    for (col = 0; col < COLUMN_COUNT; col++) {
		    SDL_FreeSurface(grid[col][row].image);
		}
	    }
}

void initParticleFX(Image particleT[]){
	init_image(&particleT[0], "Ortcrush/f1.png", 0, 0);
 	init_image(&particleT[1], "Ortcrush/f2.png", 0, 0);
 	init_image(&particleT[2], "Ortcrush/f3.png", 0, 0);
 	init_image(&particleT[3], "Ortcrush/f4.png", 0, 0);
 	init_image(&particleT[4], "Ortcrush/f5.png", 0, 0);
 	init_image(&particleT[5], "Ortcrush/f6.png", 0, 0);
 	init_image(&particleT[6], "Ortcrush/f7.png", 0, 0);
 	init_image(&particleT[7], "Ortcrush/f8.png", 0, 0);
 	init_image(&particleT[8], "Ortcrush/f9.png", 0, 0);
 	init_image(&particleT[9], "Ortcrush/f10.png", 0, 0);
 	init_image(&particleT[10], "Ortcrush/f11.png", 0, 0);
}

void displayParticleFXAnimationHorizontal(SDL_Surface *screen,  Image particleT[], int row, int col1, int colf, Text scorePopUp, int scoretoadd, SDL_Surface *gridcell, SDL_Surface *gridcellselected, Image ortcrushbg, Image starwhite1,  Image starwhite2, Image starwhite3, Text timertxt, Text scoretxt, char timertotext[], char scoretotext[], Image stargold1, Image stargold2, Image stargold3,int score, int maxScore,float totalProgress,SDL_Rect progressBarRect, SDL_Rect *progressBarScore,float *currentProgress, Candy grid[COLUMN_COUNT][ROW_COUNT]){
        int elapsedTime = 0, i=0 ,j, col, rowx;
        char scorech[10];
	Uint32 startTime = SDL_GetTicks();
	sprintf(scorech, "%d ", scoretoadd);
	sprintf(scoretotext, "%d", score);
       int star1active=2480, star2active=7080, star3active=11610;//change gold star conditions here as well as main.c
	while (i<11 && elapsedTime < 350) {
		printf("i:%d\n" ,i);
		afficher_image(ortcrushbg, screen); // Clear the screen
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
        	SDL_FillRect(screen, &progressBarRect, SDL_MapRGB(screen->format, 255, 255, 255));
		for (col = 0; col < COLUMN_COUNT; col++) {
			for (rowx = 0; rowx < ROW_COUNT; rowx++) {
				SDL_Rect destRect = {col * 100 + GRID_X, rowx * 100 + GRID_Y, 100, 100};//change screen height and width proportions
				SDL_BlitSurface(gridcell, NULL, screen, &destRect);
				if(grid[col][rowx].type!=-1){
					SDL_BlitSurface(grid[col][rowx].image, NULL, screen, &destRect);
				}
			}
		}
        	while(*currentProgress<totalProgress){
			*currentProgress += (float) totalProgress / 6;
			if (*currentProgress > totalProgress) {
				*currentProgress = totalProgress;
			}
			progressBarScore->x = ortcrushbg.pos1.x+ortcrushbg.pos1.w*0.07;
			progressBarScore->y = ortcrushbg.pos1.y+ortcrushbg.pos1.h*0.57;
			progressBarScore->w = *currentProgress*(SCREEN_W/4.3);  	
			progressBarScore->h = 30;
		    	SDL_FillRect(screen, progressBarScore, SDL_MapRGB(screen->format, 194, 255, 210));
		    	SDL_Flip(screen); 
		}//
		SDL_FillRect(screen, progressBarScore, SDL_MapRGB(screen->format, 194, 255, 210));
		
		for(j=col1;j<colf;j++){
			scorePopUp.pos_txt.x = GRID_X+j*100+50;
			scorePopUp.pos_txt.y = GRID_Y+row*100+50-5*i;
			particleT[i].pos1.x = GRID_X+j*100;
			particleT[i].pos1.y = GRID_Y+row*100-20;
			afficher_image(particleT[i], screen);
			afficher_image(particleT[i], screen);
			afficher_image(particleT[i], screen);//3 times to enhance the fx
			afficher_text(screen, &scorePopUp, scorech);
			SDL_Flip(screen);
		}
	
		SDL_Flip(screen);
		i++;
		elapsedTime = SDL_GetTicks() - startTime;
	}
	
}

void displayParticleFXAnimationVertical(SDL_Surface *screen,  Image particleT[], int col, int row1, int rowf, Text scorePopUp, int scoretoadd, SDL_Surface *gridcell, SDL_Surface *gridcellselected, Image ortcrushbg, Image starwhite1,  Image starwhite2, Image starwhite3, Text timertxt, Text scoretxt, char timertotext[], char scoretotext[], Image stargold1, Image stargold2, Image stargold3,int score, int maxScore,float totalProgress,SDL_Rect progressBarRect, SDL_Rect progressBarScore,float currentProgress, Candy grid[COLUMN_COUNT][ROW_COUNT]){
        int elapsedTime = 0, i=0 ,j, rowx, colx;
        char scorech[10];
	Uint32 startTime = SDL_GetTicks();
	sprintf(scorech, "%d ", scoretoadd);
       int star1active=2480, star2active=7080, star3active=11610;//change gold star conditions here as well as main.c
	while (i<11 && elapsedTime < 350) {
		printf("i:%d\n" ,i);
		afficher_image(ortcrushbg, screen); // Clear the screen
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
        	SDL_FillRect(screen, &progressBarRect, SDL_MapRGB(screen->format, 255, 255, 255));
		SDL_FillRect(screen, &progressBarScore, SDL_MapRGB(screen->format, 194, 255, 210));
		for (colx = 0; colx < COLUMN_COUNT; colx++) {
			for (rowx = 0; rowx < ROW_COUNT; rowx++) {
				SDL_Rect destRect = {colx * 100 + GRID_X, rowx * 100 + GRID_Y, 100, 100};//change screen height and width proportions
				SDL_BlitSurface(gridcell, NULL, screen, &destRect);
				if(grid[colx][rowx].type!=-1){
					SDL_BlitSurface(grid[colx][rowx].image, NULL, screen, &destRect);
				}
			}
		}
		
		for(j=row1;j<rowf;j++){
			scorePopUp.pos_txt.x = GRID_X+col*100+50;
			scorePopUp.pos_txt.y = GRID_Y+j*100+50-5*i;
			particleT[i].pos1.x = GRID_X+col*100;
			particleT[i].pos1.y = GRID_Y+j*100-20;
			afficher_image(particleT[i], screen);
			afficher_image(particleT[i], screen);
			afficher_image(particleT[i], screen);//3 times to enhance the fx
			afficher_text(screen, &scorePopUp, scorech);
			SDL_Flip(screen);
		}
	
		SDL_Flip(screen);
		i++;
		elapsedTime = SDL_GetTicks() - startTime;
	}
	
}
