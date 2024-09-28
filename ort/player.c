#include <SDL/SDL.h>
#include <SDL/SDL_image.h> 
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <string.h>
#include "player.h"







void init_player(Player *p, int x, int y, int max_frames, int max_rows){//use x, y differently if 2 player mode else its 0,0
	//init player pos
	p->pos.x = x;
	p->pos.y=y;
	p->prev_pos.x = x;
	p->prev_pos.y = y;
	//init player hp
	p->hp=4;
	//init player movement vars
	p->vitesse = 0;
	p->acceleration=5;//walking acceleration default
	p->vitesse_y = 0;
	p->acceleration_y = GRAVITY;
	p->lastdx = 0;
	p->lastdy=0;
	p->status = PLAYER_IDLE;
	p->laststatus = PLAYER_RIGHT;
	p->isRunning= 0;
	p->isJumping= 0;
	p->isDashing=0;
	p->isGrounded = 0;
	p->isFighting = 0;
	p->isGuarding = 0;
	p->cameraIsScrolling=0;
	p->frame=0;
	p->lastframetime=0;
	p->max_frames = max_frames;
	p->max_rows = max_rows;
	p->posRel.x = -80;
	p->isFighting=0;
	p->posRel.y= 0;
	p->lastjumpY=y;
	p->stamina=100;
	p->staminaTimer=3000;
	//p->lastacceleration=p->acceleration; //call this before and after dashing
	p->cameraPos.x=0;
	p->cameraPos.y=0;
	p->clip = NULL;
	p->currentRow=0;
	p->lastdamaged=0;//timer
	p->deathyes_txt= init_text(SCREEN_W*0.4, SCREEN_H*0.80, "fonts/emporia-roman.otf", 50, 255, 255, 255); 
	p->deathno_txt= init_text(SCREEN_W*0.57, SCREEN_H*0.80, "fonts/emporia-roman.otf", 50, 255, 255, 255); 
	p->deathscore_txt= init_text(SCREEN_W*0.5, SCREEN_H*0.925, "fonts/emporia-roman.otf", 30, 255, 255, 255); 
	//init collision logic
	p->bc=0;
	p->tc=0;
	p->lc=0;
	p->rc=0;
	p->punch_sfx = init_chunk("game/punch_sfx.wav");
}

void init_player_spritesheet(Player *p, Image spritesheets[]){//maybe add platform x, y to initialize player on it
	p->clip = (SDL_Rect **)malloc(p->max_rows * sizeof(SDL_Rect *));
	if (p->clip == NULL) {
    	// erreur mem
    		printf("Memory error while allocating array for player spritesheet (clip)\n");
	}
	for (int i = 0; i < p->max_rows; i++) {
		p->clip[i] = (SDL_Rect *)malloc(p->max_frames * sizeof(SDL_Rect));
	}
	
	for(int j=0;j<p->max_rows;j++){
		for(int i=0;i<p->max_frames;i++){
			if(j==1 || j == 5){
				p->clip[j][i].x = spritesheets[j].pos1.w / 8*i;
				p->clip[j][i].y = 0;
				p->clip[j][i].w = spritesheets[j].pos1.w/8;
				p->clip[j][i].h = spritesheets[j].pos1.h;
			
			}
			else if(j!= 10 && j!= 11){
				//if(i<=3){
					p->clip[j][i].x = spritesheets[j].pos1.w / 4*i;
					p->clip[j][i].y = 0;
					p->clip[j][i].w = spritesheets[j].pos1.w/4;
					p->clip[j][i].h = spritesheets[j].pos1.h;
				//}
			
			}
			else
			{
				p->clip[j][i].x = spritesheets[j].pos1.w / 5*i;
				p->clip[j][i].y = 0;
				p->clip[j][i].w = spritesheets[j].pos1.w/5;
				p->clip[j][i].h = spritesheets[j].pos1.h;
			
			
			}
		}
	}

}


int currentPlayerAnimation(Player p1){

	switch(p1.status){
		
		case PLAYER_DEAD:
			if(p1.laststatus==PLAYER_RIGHT)
				return 12;
			else
				return 13;
			break;
		case PLAYER_IDLE:
			if(p1.laststatus==PLAYER_RIGHT){
				if(p1.isDashing)
					return 8;
				else if(p1.isJumping)
					return 3;
				else if(p1.isFighting|| p1.isGuarding)
					return 10;
				else
					return 0;
				
			
			}
			else if(p1.laststatus == PLAYER_LEFT){
				if(p1.isDashing)
					return 9;
				else if(p1.isJumping)
					return 7;
				else if(p1.isFighting || p1.isGuarding)
					return 11;
				else
					return 4;
			}
			else
				return 4;
			
			break;
		case PLAYER_RIGHT: 
			if(p1.isDashing)
				return 8;
			else if(p1.isJumping)
				return 3;
			else if(p1.isRunning)
				return 2;
			else if(p1.isFighting || p1.isGuarding)
				return 10;
			else
				return 1;
			break;
		case PLAYER_LEFT: 
			if(p1.isDashing)
				return 9;
			else if(p1.isJumping)
				return 7;
			else if(p1.isRunning)
				return 6;
			else if(p1.isFighting || p1.isGuarding)
				return 11;
			else
				return 5;
			break;
		default:
			return 0;
		
	
	}







}

void move_player(Player *p, Uint32 dt){
	double dx=0;
	if(p->vitesse>10){
		p->vitesse=10;
	}
	else if(p->vitesse<0){
		p->vitesse=0;
	}
	p->prev_pos.x = p->pos.x;
	
	if(p->status == PLAYER_RIGHT){
		if (p->isDashing) {
			p->dashTimer -= dt;
			if (p->dashTimer <= 0) {
			    p->isDashing = 0;
			    if(p->isRunning)
			    	p->acceleration  = 20;//running
			    else
			    	p->acceleration=5;//walking
			}
	   	 } 
		dx = 0.5 * p->acceleration* dt*dt + p->vitesse*dt;
		if(!p->cameraIsScrolling && !p->rc )
			p->pos.x += dx/1000.0;	
		p->lastdx = dx/1000.0;
	//p->vitesse
	}
	else if(p->status == PLAYER_LEFT){
		if (p->isDashing) {
			p->dashTimer -= dt;
			if (p->dashTimer <= 0) {
			    p->isDashing = 0;
			    
			    if(p->isRunning)
			    	p->acceleration  = 20;//running
			    else
			    	p->acceleration=5;//walking
			}
	   	 } 
		dx = 0.5 * p->acceleration* dt*dt + p->vitesse*dt;
		if(!p->cameraIsScrolling && !p->lc)
			p->pos.x -= dx/1000.0;
		
		p->lastdx = -dx/1000.0;
	}
	else if(p->status == PLAYER_IDLE){
		if (p->isDashing) {
			p->dashTimer -= dt;
			if (p->dashTimer <= 0) {
			    p->isDashing = 0;
			    if(p->isRunning)
			    	p->acceleration  = 20;//running
			    else
			    	p->acceleration=5;//walking
			}
		   	 dx = 0.5 * p->acceleration* dt*dt + p->vitesse*dt;
		   	 if(!p->cameraIsScrolling){
				if(p->laststatus==PLAYER_RIGHT && !p->rc)
					p->pos.x += dx/1000.0;
				else if(p->laststatus==PLAYER_LEFT&& !p->lc)
					p->pos.x -= dx/1000.0;
			}
			p->lastdx = dx/1000.0;
	   	 } 
	
	
	}
	printf("acceleration: %.2f | vitesse: %.2f | dx: %.2f\n", p->acceleration, p->vitesse, dx);
	//printf("pos x: %d\n", p->pos.x);
	
	
	//handle stamina value
	if(p->status != PLAYER_IDLE && p->isRunning){
		if (p->stamina > 0) {
		        p->stamina -= dt/1000.0; 
		        if (p->stamina < 0) {
		            p->stamina = 0; 
		        }
		    }
		else{
			p->isRunning=0;
			p->acceleration=5;
			
		
		}

           }
           
          
         if(p->stamina<100){
         	if(!p->isRunning){
			p->staminaTimer -=dt;
		}
		else
			p->staminaTimer = 3000;//RESETS TIMER IF PLAYER RUNS. PLAYER shouldnt run FOR 3 SECONDS TO REPLENISH STAMINA
		if(p->staminaTimer<=0){
			p->stamina +=2;//replenish bar if 3 seconds passed
			if(p->stamina >= 100){
				p->stamina=100;
				p->staminaTimer=3000;
			}
		}	
		
		
	}
	

		
}


void display_player(Player p, SDL_Surface* ecran, Image spritesheet, SDL_Rect* clip){
	if(SDL_BlitSurface(spritesheet.img, clip, ecran, &p.pos)<0){
		printf("%s\n", SDL_GetError());
	}
}

void handlePlayerKeyDownEvent(Player *p1, SDL_Event event,  Uint8 keystate[]){
	switch(event.key.keysym.sym){
		case SDLK_d:
			p1->status = PLAYER_RIGHT;
			p1->isFighting=0;
			p1->isGuarding=0;
			p1->laststatus = p1->status;
			break;
		case SDLK_q:
			p1->status = PLAYER_LEFT;
			p1->isFighting=0;
			p1->isGuarding=0;
			p1->laststatus = p1->status;
			break;
		case SDLK_SPACE:
			if(!p1->isJumping && p1->bc){
				p1->isJumping=1;
				p1->lastjumpY=p1->pos.y;
			}
			p1->isFighting=0;
			p1->isGuarding=0;
			break;
		case SDLK_LSHIFT:
			if(p1->stamina!=0){
				p1->isRunning=1;
				p1->acceleration = 20; //running acceleration
			}
			p1->isFighting=0;
			p1->isGuarding=0;
			break;				
		case SDLK_f:
			if (!p1->isDashing && p1->stamina!=0 && p1->stamina-20 >=0) {
				p1->isDashing = 1;
				p1->acceleration = 200;
				p1->dashTimer = 100;
				p1->stamina-=20;//takes 20 off stamina
			    }
			p1->isFighting=0;
			p1->isGuarding=0;
			break;
		case SDLK_b:
			if(p1->status==PLAYER_IDLE){
				p1->isFighting=1;
				p1->frame = 0;	
			}
			p1->isGuarding=0;
			break;
		case SDLK_LCTRL:
			if(p1->status==PLAYER_IDLE){
				p1->isGuarding=1;
				p1->isFighting=0;
				p1->frame = 0;
				
			}
			break;
		    				
    		}

}



void handlePlayerKeyUpEvent(Player *p1, SDL_Event event, Uint8 keystate[]){
	switch(event.key.keysym.sym){
	    			case SDLK_d:
	    			case SDLK_q:
	    				p1->status = PLAYER_IDLE;
	    				p1->frame=0;
	    				break;
				case SDLK_LSHIFT:
					p1->isRunning=0;
					p1->acceleration = 5; //walk acceleration
					break;
				case SDLK_b:
					p1->isFighting=0;
					break;
				case SDLK_LCTRL:
					p1->isGuarding=0;
					break;

	    		}




}


void handlePlayerKeyHeldEvent(Player *p1, Uint8 keystate[], Uint32 dt){
	if(keystate[SDLK_d] && !keystate[SDLK_q]){
		// d key pressed
		p1->vitesse+=p1->acceleration*dt/1000.0;
	}
	else if(keystate[SDLK_q] && !keystate[SDLK_d]){
		// q key pressed
		p1->vitesse+=p1->acceleration*dt/1000.0;
	}
	else if((!keystate[SDLK_q] && !keystate[SDLK_d]) || (keystate[SDLK_q] && keystate[SDLK_d])){
		//if both q and d are pressed-> stop the player
		if(p1->vitesse!=0){
			p1->vitesse-=p1->acceleration*dt/1000.0;
			if(p1->vitesse<0)
				p1->vitesse=0;
			if(p1->vitesse==0)
				p1->status=PLAYER_IDLE;
			}
	
	}
	//printf("vitesse: %.2f\n", p1->vitesse);

}

void updatePlayerHW(Player *p, SDL_Rect currentClip){
	//p->playerW = currentClip.w;//-0.08*currentClip.w;
	p->playerW = 90;
	p->playerH = 191;//currentClip.h;
	//printf("Current player Width: %d | Height: %d\n", p->playerW, p->playerH); 

}

void apply_gravity(Player *p, Uint32 dt) {//implement this when collision function returns smth != 1(platform collision = 1)=> player is not on platform and not colliding with anything =>apply gravity
	// Apply gravity only if the player is not jumping
	// Check if the player is not colliding with anything below and is not grounded
	if (p->isJumping==0 && p->bc==0) {
		double gravitationalForce = GRAVITY * 50; // F = m * g (mass * gravity)
		//
		p->acceleration_y = gravitationalForce / 50; // F = m * a, so a = F / m
		// Update the player's velocity based on acceleration
		p->vitesse_y += p->acceleration_y * dt / 1000.0; // Update player velocity (v = u + at)
		//
		double dy = p->acceleration_y*dt*dt*0.5 + p->vitesse*dt;
		//
		p->prev_pos.y = p->pos.y;
		p->pos.y += dy/1000.0; // Update player position (s = ut + 0.5 * at^2)
		p->prev_pos.y = dy/1000.0;
		p->lastdy = dy/1000.0;
	}
	else{
		p->vitesse_y=0;
	}
	//printf("vitesse y: %.2f\n dt: %d\n", p->vitesse_y, dt);

}
void handlePlayerAnimation(Player *p1, Uint32 currentTime, Uint32 dt){
	if (p1->hp<=0)
	{
		p1->hp=0;
		p1->laststatus = p1->status;
		p1->status = PLAYER_DEAD;
	}
	if(p1->status==PLAYER_DEAD && currentTime-p1->lastframetime>150){
		if(p1->frame<4)
			p1->frame++;
		p1->lastframetime=currentTime;
	}
	else if(p1->status!=PLAYER_IDLE && currentTime-p1->lastframetime>150){
		if(p1->currentRow == 1 && p1->currentRow==5)
			p1->frame = (p1->frame + 1) % 8;
		else
			p1->frame = (p1->frame + 1) % 4;
		//else
		//	p1->frame = (p1->frame + 1) % 5;
		/*if(p1->frame<p1->max_frames-1)
			p1->frame++;
		else
			p1->frame=0;*/
		p1->lastframetime=currentTime;
	}
	else if(p1->status==PLAYER_IDLE && currentTime-p1->lastframetime>150){
		if(p1->isFighting)
			p1->frame = (p1->frame + 1) % 4;
		else if(p1->isGuarding){
			p1->frame=0;
		}
		
		
		//else
		//	p1->frame = (p1->frame + 1) % 5;
		/*if(p1->frame<p1->max_frames-1)
			p1->frame++;
		else
			p1->frame=0;*/
		p1->lastframetime=currentTime;
	}
	printf("player status: %d | player frame: %d\n", p1->status, p1->frame);

	

}

void jump(Player *p, double dt, int posAbs_x, int posAbs_y)
{
	int k=5;

	if(p->isJumping== 1)
	{
			if(p->posRel.x <=80){
				p->posRel.x += k;
			}
		 	if(p->posRel.x >=80)
			{
			    p->posRel.x = -80;
			}
		//p->posRel.y = ( -0.03125* (p->posRel.x*p->posRel.x)+ 200);
		
		p->posRel.y = ( -0.02109375 * (p->posRel.x*p->posRel.x)+ 135);
		//p->posRel.y = ( -0.015625 * (p->posRel.x*p->posRel.x)+ 100);
		if(p->posRel.x >0){
			k=1;
			p->lastdy = p->posRel.y;
		}
		else
			p->lastdy = -p->posRel.y;


		p->prev_pos.y = p->pos.y;
		p->pos.y = posAbs_y - p->posRel.y;
		//printf("pos rel: %d | pos y: %d\n", p->posRel.y, p->pos.y);

		if( p->pos.y >= posAbs_y){
			p->pos.y = posAbs_y;
			p->isJumping = 0;
			p->posRel.y = 0;
			//p->isGrounded=0;

		}
	}
	else
		p->lastdy = 0;
}

void displayStaminaBar(Player p, Image staminaimg, SDL_Surface *screen){
	SDL_Rect clip;
	int remainingWidth = (p.stamina * staminaimg.img->w) / 100;
	int startX = staminaimg.img->w-remainingWidth; 
	clip.x = startX;
	clip.y=0;
	clip.w = remainingWidth; 
        clip.h = staminaimg.img->h;
        afficher_sprite_at_pos(staminaimg, screen, &clip, &(SDL_Rect){clip.x+staminaimg.pos1.x, staminaimg.pos1.y});
        //afficher_sprite(staminaimg, screen, &clip);


}


void liberer_player(Player p){
	//liberer_image(p.spritesheet);
	liberer_image(p.deathscreen);
	liberer_text(&p.deathyes_txt);
	liberer_text(&p.deathno_txt);
	liberer_text(&p.deathscore_txt);
	liberer_chunk(p.punch_sfx);
	free(p.clip);

}

