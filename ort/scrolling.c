#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "scrolling.h"


// Initialize camera
void initCamera(Camera *camera, int startX, int startY, int screenWidth, int screenHeight, int smoothness) {
    camera->pos.x = startX;
    camera->pos.y = startY;
    camera->initY = startY;
    camera->pos.w = screenWidth;
    camera->pos.h = screenHeight;
    camera->directionX = -1; 
    camera->directionY = -1; 
    camera->dx = 0;
    camera->dy = 0;
    camera->smoothness = smoothness;
}

// Update camera position

void updateCamera(Camera *camera, Player *p, int screenWidth, int screenHeight, int bgWidth, int bgHeight) {

    //ken player binet screen_w/3 w screen_w/2=> ysir ken deplacement (p->cameraIsScrolling=0 => player velocity is active again)
    //ken player f khat mtaa screen_w/3 w mechi alisar => scrolling left
    //ken player f khat mtaa screen_w/2 w mechi alimin => scrolling right
    //maybe make camera zoom out when player runs to give it a better feel
    
    //TRANSLATION X
    p->cameraIsScrolling = 0;
    //RIGHT
    if (camera->directionX==1 && p->pos.x >= screenWidth / 2 && p->pos.x < bgWidth - screenWidth / 2) {
        printf("camera moving: cam x: %d || last dx: %d \n", camera->pos.x, camera->dx);
        camera->pos.x += camera->dx;//player dx is positive	
        p->cameraIsScrolling = 1;

   }
	//LEFT
   if (camera->directionX==2 && p->pos.x <= screenWidth /3) {
        printf("camera moving: cam x: %d || last dx: %d \n", camera->pos.x, camera->dx);
        
        camera->pos.x += camera->dx;//player dx is negative
	p->cameraIsScrolling = 1;
   } 

        // constraints to ensure the camera stays within background image borders horizontally
        if (camera->pos.x < 0) {
            camera->pos.x = 0;
            p->cameraIsScrolling=0;
        } else if (camera->pos.x + camera->pos.w > bgWidth) {
            camera->pos.x = bgWidth - camera->pos.w;
            p->cameraIsScrolling=0;	
        }
        
        //TRANSLATION Y
   //if (p->pos.y < screenHeight / 2 /*&& playerY < bgHeight - screenHeight / 2*/){
 	int player_center_y = p->pos.y + p->playerH / 2;

    	int camera_y = player_center_y - 540;

    // center cam
    	//camera->pos.y = camera_y;
	if(p->isJumping)
		camera->pos.y += p->lastdy*0.08;
  	/*else if(camera->pos.y != camera->initY){
  		camera->pos.y -= p->lastdy*0.02;
  		if(camera->pos.y < camera->initY){
  			camera->pos.y = camera->initY;
  		}
  	}*/
  	if(!p->isJumping)
  		camera->pos.y = 500;
        //camera->pos.y = camera_y;
  	 printf("camera moving: cam y: %d || last dy: %d \n", camera->pos.y, camera->dy);


        // VERTICAL CONSTRAINTS
       if (camera->pos.y < 0) {
            camera->pos.y = 0;

        } else if (camera->pos.y + 1080 > bgHeight) {
            camera->pos.y = bgHeight - camera->pos.h;
        }
        
       	
        p->cameraPos.x=camera->pos.x;
        p->cameraPos.y=camera->pos.y;
    }


void updateCameraVertical(Camera *camera, Player *p, int screenWidth, int screenHeight, int bgWidth, int bgHeight){
	 // Vertical Scrolling
	  int player_center_y = p->pos.y  + p->playerH / 2;
	int camera_y = player_center_y - screenHeight / 2;

    // Move camera vertically based on player's position
    if (player_center_y > screenHeight / 2 && player_center_y < bgHeight - screenHeight / 2) {
        camera->pos.y = camera_y;
    }

    // Vertical constraints to ensure the camera stays within background image borders
    if (camera->pos.y < 0) {
        camera->pos.y = 0;
    } else if (camera->pos.y + camera->pos.h > bgHeight) {
        camera->pos.y = bgHeight - camera->pos.h;
    }


}


void scrollHorizontal(Camera *camera, int direction, int step) {
    camera->directionX = direction;
    camera->dx = step / camera->smoothness; // Divide step by smoothness for smooth movement
}

void scrollVertical(Camera *camera, int direction, int step) {
    camera->directionY = direction;
    camera->dy = step / camera->smoothness; // Divide step by smoothness for smooth movement
}
void renderBackground(SDL_Surface *screen, Image background, Image *maskedBackground, Camera *camera, Player *p) {
    SDL_Rect srcRect = {camera->pos.x, camera->pos.y, camera->pos.w, camera->pos.h};
    maskedBackground->pos2.x = camera->pos.x;
    maskedBackground->pos2.y = camera->pos.y;
    p->cameraPos.x = camera->pos.x;
    p->cameraPos.y = camera->pos.y;
    //p->pos.x -= camera->pos.x;
    //p->pos.y -= camera->pos.y;
    maskedBackground->pos2.w = 1920;
    maskedBackground->pos2.h = 1080;
    SDL_Rect destRect = {0, 0, camera->pos.w, camera->pos.h};
   SDL_BlitSurface(background.img, &srcRect, screen, &destRect);

   // SDL_BlitSurface(maskedBackground->img, &maskedBackground->pos2, screen, &maskedBackground->pos1);
}

/*void renderBackground(SDL_Surface *screen, Image background[], Image maskedBackground[], int nbg, int nbgmask, Camera *camera, Player *p) {
    SDL_Rect srcRect = {camera->pos.x, camera->pos.y, camera->pos.w, camera->pos.h};
    for(int i=0;i<nbg;i++){
    
    
    }
    maskedBackground->pos2.x = camera->pos.x;
    maskedBackground->pos2.y = camera->pos.y;
    p->cameraPos.x = camera->pos.x;
    p->cameraPos.y = camera->pos.y;
    //p->pos.x -= camera->pos.x;
    //p->pos.y -= camera->pos.y;
    maskedBackground->pos2.w = 1920;
    maskedBackground->pos2.h = 1080;
    SDL_Rect destRect = {0, 0, camera->pos.w, camera->pos.h};
    SDL_BlitSurface(maskedBackground->img, &maskedBackground->pos2, screen, &maskedBackground->pos1);
    //SDL_BlitSurface(background.img, &srcRect, screen, &destRect);
}
*/
