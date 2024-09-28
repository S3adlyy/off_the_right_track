#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "collision.h"







SDL_Color GetPixel(SDL_Surface *pSurface,int x,int y) { 
	SDL_Color color; 
	Uint32 col=0; 
	//Determine position 
	char* pPosition=(char* ) pSurface->pixels;
	pPosition+= (pSurface->pitch * y);
	pPosition+= (pSurface->format->BytesPerPixel *x);
	memcpy(&col ,pPosition ,pSurface->format->BytesPerPixel);
	//convertir color 
	SDL_GetRGB(col,pSurface->format, &color.r, &color.g, &color.b);
	return (color);
}

int CheckCollision(Image *mask, int x, int y) {
    SDL_Color pixel = GetPixel(mask->img, x, y);
   printf("COLOR COLLIDED (r, g, b) = (%d,%d,%d)\n", pixel.r, pixel.g, pixel.b);
    //return (0<=pixel.r<= 3 && 0<=pixel.g<= 3 && 0<=pixel.b<= 3); // Collision if color is black
    //return ((pixel.r == 0 && pixel.g == 0 && pixel.b == 0)||(pixel.r == 0 && pixel.g == 1 && pixel.b == 0)||(pixel.r == 0 && pixel.g == 1 && pixel.b == 1)||(pixel.r == 1 && pixel.g == 0 && pixel.b == 0)||(pixel.r == 0 && pixel.g == 0 && pixel.b == 2));
    if(pixel.r == 255 && pixel.g == 0 && pixel.b == 0)//red
    	return 2;//damage
    else if((pixel.r == 0 && pixel.g == 255 && pixel.b == 0)||(pixel.r == 1 && pixel.g == 255 && pixel.b == 0)||(pixel.r == 2 && pixel.g == 255 && pixel.b == 1)||(pixel.r == 1 && pixel.g == 255 && pixel.b == 1)||(pixel.r == 0 && pixel.g == 255 && pixel.b == 2))//green
    	return 1;//collision
    else if(pixel.r == 0 && pixel.g == 0 && pixel.b == 0)
    	return 3; //Kill
    return 0;
}


int FindNearestValidY(Image *mask, Player *p, Camera camera, int x , int y, int dir) {
    // Start checking for collision from the player's current position
    //x = p->pos.x + p->playerW / 2 + camera.pos.x;
    //y = p->pos.y + p->playerH + camera.pos.y;
    //int y2=y;
    while (CheckCollision(mask, x, y-1)) {
        y=y+dir; // Move one pixel up
    }
    // Return the nearest valid Y position
    //printf("VALID Y: %d \n", y);
    return y;
}

int FindNearestValidX(Image *mask, Player *p, Camera camera, int x , int y, int dir) {
    // Start checking for collision from the player's current position
    x = p->pos.x + p->playerW + camera.pos.x;
    y = p->pos.y + p->playerH /2+ camera.pos.y;
    /*if(dir==-1)
    	int k = y + p->playerH + camera.pos.y;
    else
    	int k = y*/

    // Start tracing upward from the bottom-center point
    while (CheckCollision(mask, x, y)) {
        x=x+dir; // Move one pixel 
    }
    

    // Return the nearest valid Y position
    //printf("VALID X: %d \n", x);
    return x;
}

//////////---------------------------- BOUNDING BOX COLLISION SYSTEM


int checkCollision(SDL_Rect a, SDL_Rect b) {
    // Check if a and b overlap
    if (a.x + a.w < b.x || a.x > b.x + b.w || a.y + a.h < b.y || a.y > b.y + b.h) {
        return 0; // No collision
    } else {
        return 1; // Collision detected
    }
    //return(a.x + a.w < b.x || a.x > b.x + b.w || a.y + a.h < b.y || a.y > b.y + b.h);
}


void handlePlayerCollision(Player *player, Image *obstacles, int numObstacles, Camera camera, SDL_Surface *ecran) {//allocate obstacles array or just declare a normal array
	SDL_Rect playerRect = player->pos, playerRight, playerBottom, playerTop, playerLeft;
	
	//collision logic on all sides
	player->bc = 0;
	player->tc = 0;
	player->lc = 0;
	player->rc = 0;
	player->isGrounded=0;
	

	for (int i = 0; i < numObstacles; i++) {
		SDL_Rect obstacleRect = obstacles[i].pos1;
		//printf("obstacle y: %d\n", obstacleRect.y);
		SDL_Rect playerBottom = {playerRect.x+25, playerRect.y + player->playerH - 20, player->playerW-30, 20};
		// Check collision on bottom side of player
		if (checkCollision(playerBottom, obstacleRect)!=0) {
		    // Resolve collision on bottom side
		    // Example: stop player from falling through platforms
		    printf("BOTTOM COLLISION AT: p pos.y: %d || player bottom rect.y: %d || obstacle y: %d\n", playerRect.y, playerBottom.y, obstacleRect.y);
		    player->vitesse_y = 0;
		    player->isGrounded=1;
		    player->bc = 1;
		    printf("i: %d\n",i);
		    player->pos.y = obstacleRect.y-player->playerH;
		    playerRect = player->pos;
		    //break;
		 
		}

		SDL_Rect playerTop = {playerRect.x+20, playerRect.y, player->playerW-40, 10};
		// Check collision on top side of player
		if (checkCollision(playerTop, obstacleRect)) {
		    // Resolve collision on top side
		    // Example: stop player from moving upwards
		       //player->vitesse_y = 0;
		       printf("TOP COLLISION AT: p pos.y: %d || player top rect.y: %d || obstacle y: %d\n", playerRect.y, playerTop.y, obstacleRect.y);
		       //printf("TOP COLLISION\n");
		       player->tc = 1;
		       printf("i: %d\n",i);
		       //player->pos.y = obstacleRect.y+obstacleRect.h;
		       playerRect = player->pos;
		      // break;
		}
		
		SDL_Rect playerLeft = {playerRect.x, playerRect.y+25, 10, player->playerH-75};
		if (checkCollision(playerLeft, obstacleRect)!=0) {
		    // Resolve collision on left side
		    // Example: stop player from moving left
		    printf("LEFT COLLISION AT: p pos.X: %d || player left rect.X: %d || obstacle X: %d\n", playerRect.x, playerLeft.x, obstacleRect.x);
		    printf("i: %d\n",i);
		     player->vitesse = 0;
		     player->lc = 1;
		     player->pos.x = obstacleRect.x+obstacleRect.w;
		     playerRect = player->pos;
		     //break;
		}

		SDL_Rect playerRight = {playerRect.x + player->playerW - 10, playerRect.y+25, 10, player->playerH-75};
		// Check collision on right side of player
		if (checkCollision(playerRight, obstacleRect)) {
		   // Resolve collision on right side
		   // Example: stop player from moving right
		   // printf("RIGHT COLLISION\n");
		   printf("i: %d\n",i);
		   printf("RIGHT COLLISION AT: p pos.X: %d || player right rect.X: %d || obstacle X: %d\n", playerRect.x, playerRight.x, obstacleRect.x);
		    player->vitesse = 0;
		    player->rc = 1;
		    player->pos.x = obstacleRect.x-player->playerW;
		    playerRect = player->pos;
		    //break;
		}
		
	}
	
	SDL_FillRect(ecran, &playerBottom, SDL_MapRGB(ecran->format, 255, 255, 255));
	SDL_FillRect(ecran, &playerTop, SDL_MapRGB(ecran->format, 255, 255, 255));
	SDL_FillRect(ecran, &playerLeft, SDL_MapRGB(ecran->format, 255, 255, 255));
	SDL_FillRect(ecran, &playerRight, SDL_MapRGB(ecran->format, 255, 255, 255));
	SDL_Flip(ecran);
}



void updateObstaclesWithCamera(Image* obstacles, int num_obstacles,Camera camera){
	for(int i=0;i<num_obstacles;i++){
		obstacles[i].pos1.x -= camera.pos.x;
	}

}



/////////////-------PERFECT PIXEL COLLISION V2--------
int CollisionDetectionLeft(Image *mask, Player *p, Camera camera) {
    
	p->lc = 0;
	// Initialize 3 points for the left side
	int points[3][2] = {
		{p->pos.x + camera.pos.x, p->pos.y + camera.pos.y + p->playerH*0.2},                        // Top-left 
		{p->pos.x+ camera.pos.x, p->pos.y + p->playerH * 0.5+ camera.pos.y},      // Middle-left
		{p->pos.x + camera.pos.x, p->pos.y + p->playerH *0.8+ camera.pos.y} // Bottom-left
	};

	// Check collision for the left side
	for (int i = 0; i < 3; i++) {
		int x = points[i][0];
		int y = points[i][1];
		int c = CheckCollision(mask, x, y);
		if (c!=0) {
			p->pos.x = FindNearestValidX(mask, p, camera, x, y, 1) - p->playerW - camera.pos.x;
			p->lc = 1;
			p->vitesse =0;
			p->acceleration=5;
			return c;
		}
	}

}

int CollisionDetectionRight(Image *mask, Player *p, Camera camera) {
	p->rc = 0;
	// Initialize 3 points for the right side
	int points[3][2] = {
		{p->pos.x + p->playerW + camera.pos.x, p->pos.y + camera.pos.y + p->playerH*0.2},           // Top-right
		{p->pos.x + p->playerW + camera.pos.x, p->pos.y + p->playerH * 0.5 + camera.pos.y}, // Middle-right
		{p->pos.x + p->playerW + camera.pos.x, p->pos.y + p->playerH*0.8 + camera.pos.y} // Bottom-right
	};

	// Check collision for the right side
	for (int i = 0; i < 3; i++) {
		int x = points[i][0];
		int y = points[i][1];
		int c = CheckCollision(mask, x, y);
		if (c!=0) {
			p->pos.x = FindNearestValidX(mask, p, camera, x, y, -1) - p->playerW - camera.pos.x;
			printf("POINT I:%d | NEW POS X: %d\n", i, p->pos.x);
			p->rc=1;
			p->vitesse =0;
			p->acceleration=5;
			return c;
		}
	}


}

int CollisionDetectionTop(Image *mask, Player *p, Camera camera) {
	p->tc=0;
	// Initialize 3 points for the top side
	int points[3][2] = {
		{p->pos.x+ camera.pos.x + p->playerW * 0.2, p->pos.y + camera.pos.y},                        // Top-left 
		{p->pos.x + p->playerW * 0.5 + camera.pos.x, p->pos.y + camera.pos.y},      // Top-center
		{p->pos.x + p->playerW *0.8 +camera.pos.x, p->pos.y + camera.pos.y}           // Top-right
	};

	// Check collision for the top side
	for (int i = 0; i < 3; i++) {
		int x = points[i][0];
		int y = points[i][1];
		int c = CheckCollision(mask, x, y);
		if (c!=0) {
			//p->pos.y = FindNearestValidY(mask, p, camera, x, y, 1) + p->playerH + camera.pos.y;
			//p->pos.y += p->lastdy;
			if(p->isJumping){
				p->isJumping=0;
				p->posRel.x = -80;
			}
			p->tc = 1;
			return c;
		}
	}

}

int CollisionDetectionBottom(Image *mask, Player *p, Camera camera) {
	p->bc = 0;
	// Initialize 3 points for the bottom side
	int points[3][2] = {
		{p->pos.x + p->playerW *0.2+ camera.pos.x, p->pos.y + p->playerH + camera.pos.y}, // Bottom-3
		{p->pos.x + p->playerW *0.5 + camera.pos.x, p->pos.y + p->playerH + camera.pos.y}, // Bottom-center
		{p->pos.x + p->playerW *0.8+camera.pos.x, p->pos.y + p->playerH + camera.pos.y}  // Bottom-1
	};

	// Check collision for the bottom side
	for (int i = 0; i < 3; i++) {
		int x = points[i][0];
		int y = points[i][1];
		int c = CheckCollision(mask, x, y);
		if (c!=0) {
			printf("POINT I:%d | OLD POS Y: %d\n", i, p->pos.y);
			p->pos.y = FindNearestValidY(mask, p, camera, x, y, -1) - p->playerH - camera.pos.y;
			printf("POINT I:%d | NEW POS Y: %d\n", i, p->pos.y);
			if(p->isJumping && p->posRel.x > 0){
				p->isJumping=0;
			//	p->posRel.y = 0;
				p->posRel.x = -80;
			}
			p->bc = 1;
			return c;
		}
	}

}




