#include <SDL/SDL.h>
#include <SDL/SDL_image.h> 
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "enemy.h"
#include "fonctions.h"
#include "player.h"


void init_enemy(Enemy *e, int x, int y){
	e->hp = 12;
	e->pos.x = x;
	e->pos.y = y;
	e->posInit.x = x;
	e->posInit.y = y;
	e->enemyReturnTimer=2000;
	e->fightTimer = 1500;
	e->maxrange = 150;
	//init enemy health bar
	e->hpbarprogress.x = x+50;
	e->hpbarprogress.y = y-40;
	e->hpbarprogress.w = 100;
	e->hpbarprogress.h = 10;
	e->hpbar.x = x+50;
	e->hpbar.y = y-40;
	e->hpbar.w = 100;
	e->hpbar.h = 10;
	init_image(&e->spritesheets[0], "mummy_walk_right.png", x, y);//walk right
	init_image(&e->spritesheets[1], "mummy_fight_right.png", x, y);//run/fight right
	init_image(&e->spritesheets[2], "mummy_walk_right.png", x, y);//death right
	init_image(&e->spritesheets[3], "mummy_walk_left.png", x, y);//walk left
	init_image(&e->spritesheets[4], "mummy_fight_left.png", x, y);//run-fight left
	init_image(&e->spritesheets[5], "mummy_walk_right.png", x, y);//death left
	/*for(int i=0;i<3;i++){
		for(int j=0;j<4;j++){
			e->rightframes[j][i].x = j * e->spritesheets[i].img->w / 4;
			e->rightframes[j][i].y = 0;
			e->rightframes[j][i].w = e->spritesheets[i].img->w / 4;
			e->rightframes[j][i].h = e->spritesheets[i].img->h;
		}
	
	}
	int k=3;
	for(int i=0;i<3;i++){
		for(int j=0;j<4;j++){
			e->leftframes[j][i].x = j * e->spritesheets[k].img->w / 4;
			e->leftframes[j][i].y = 0;
			e->leftframes[j][i].w = e->spritesheets[k].img->w / 4;
			e->leftframes[j][i].h = e->spritesheets[k].img->h;
		}
		k++;
	}*/
	for(int i=0;i<3;i++){
		for(int j=0;j<4;j++){
			e->rightframes[i][j].x = j * e->spritesheets[i].img->w / 4;
			e->rightframes[i][j].y = 0;
			e->rightframes[i][j].w = e->spritesheets[i].img->w / 4;
			e->rightframes[i][j].h = e->spritesheets[i].img->h;
		}
	
	}
	int k=3;
	for(int i=0;i<3;i++){
		for(int j=0;j<4;j++){
			e->leftframes[i][j].x = j * e->spritesheets[k].img->w / 4;
			e->leftframes[i][j].y = 0;
			e->leftframes[i][j].w = e->spritesheets[k].img->w / 4;
			e->leftframes[i][j].h = e->spritesheets[k].img->h;
		}
		k++;
	}

	e->status = ENEMY_IDLE;
	e->frame=0;
	e->laststatus = ENEMY_LEFT;
	e->isFighting=0;
	e->punched=0;
	e->lastframetime =0;
	e->lastdamagetime =0;
	e->pos.w = e->spritesheets[0].img->w / 4;
	e->pos.h = e->spritesheets[0].img->h;

}

void afficher_enemy(Enemy e, SDL_Surface *ecran, Camera camera){
	e.pos.x -= camera.pos.x;
	e.pos.y -= camera.pos.y - 500 ;//500 is init cam pos
	e.hpbarprogress.x = e.pos.x+50;
	e.hpbarprogress.y = e.pos.y-40;
	e.hpbar.x = e.pos.x+50;
	e.hpbar.y = e.pos.y-40;
	//printf("enemy frame: %d | enemy pos rect clip x: %d | enemy pos rect clip w: %d | pos x: %d | pos y: %d \n", e.frame, e.leftframes[0][e.frame].x, e.leftframes[0][e.frame].w, e.pos.x, e.pos.y);
	switch(e.status){
		case ENEMY_IDLE:
			if(e.laststatus==ENEMY_RIGHT)
				afficher_sprite_at_pos(e.spritesheets[0], ecran, &e.rightframes[0][0], &e.pos);
			else if(e.laststatus==ENEMY_LEFT)
				afficher_sprite_at_pos(e.spritesheets[3], ecran, &e.leftframes[0][0], &e.pos);
			else
				afficher_sprite_at_pos(e.spritesheets[4], ecran, &e.leftframes[1][0], &e.pos);
			break;
		case ENEMY_RIGHT:
			if(e.isFighting)
				afficher_sprite_at_pos(e.spritesheets[1], ecran, &e.rightframes[1][e.frame], &e.pos);
			else
				afficher_sprite_at_pos(e.spritesheets[0], ecran, &e.rightframes[0][e.frame], &e.pos);
			break;
		case ENEMY_LEFT:
			if(e.isFighting)
				afficher_sprite_at_pos(e.spritesheets[4], ecran, &e.leftframes[1][e.frame], &e.pos);
			else
				afficher_sprite_at_pos(e.spritesheets[3], ecran, &e.leftframes[0][e.frame], &e.pos);
			break;
		/*case ENEMY_DEAD:
			if(e.laststatus==ENEMY_RIGHT)
				afficher_sprite_at_pos(e.spritesheets[2], ecran, &e.rightframes[e.frame][2], &e.pos);
			else if(e.laststatus==ENEMY_LEFT)
				afficher_sprite_at_pos(e.spritesheets[2], ecran, &e.leftframes[e.frame][2], &e.pos);
			break;*/
		//default:
		//	afficher_sprite_at_pos(e.spritesheets[0], ecran, &e.leftframes[0][e.frame], &e.pos);
			
			
	}
	
	/*switch(e.status){
	
		case ENEMY_IDLE:
			if(e.laststatus==ENEMY_RIGHT)
				afficher_sprite_at_pos(e.spritesheets[0], ecran, &e.rightframes[0][2], &e.pos);
			else if(e.laststatus==ENEMY_LEFT)
				afficher_sprite_at_pos(e.spritesheets[3], ecran, &e.leftframes[0][2], &e.pos);
			break;
		case ENEMY_RIGHT:
			if(e.isFighting)
				afficher_sprite_at_pos(e.spritesheets[1], ecran, &e.rightframes[e.frame][1], &e.pos);
			else
				afficher_sprite_at_pos(e.spritesheets[0], ecran, &e.rightframes[e.frame][0], &e.pos);
			break;
		case ENEMY_LEFT:
			if(e.isFighting)
				afficher_sprite_at_pos(e.spritesheets[4], ecran, &e.leftframes[e.frame][1], &e.pos);
			else
				afficher_sprite_at_pos(e.spritesheets[3], ecran, &e.leftframes[e.frame][0], &e.pos);
			break;
		case ENEMY_DEAD:
			if(e.laststatus==ENEMY_RIGHT)
				afficher_sprite_at_pos(e.spritesheets[2], ecran, &e.rightframes[e.frame][2], &e.pos);
			else if(e.laststatus==ENEMY_LEFT)
				afficher_sprite_at_pos(e.spritesheets[2], ecran, &e.leftframes[e.frame][2], &e.pos);
			break;
		default:
			afficher_sprite_at_pos(e.spritesheets[0], ecran, &e.leftframes[e.frame][0], &e.pos);
			
			
	}*/
	if(e.status!=ENEMY_DEAD || (e.status==ENEMY_DEAD && e.frame>4) ){
	SDL_FillRect(ecran, &e.hpbar, SDL_MapRGB(ecran->format, 255, 255, 255));
	SDL_FillRect(ecran, &e.hpbarprogress, SDL_MapRGB(ecran->format, 255, 0, 0));
	}
	
}


void move_enemy(Enemy *e, Player p, Uint32 dt, Camera camera){
	
	
		if(e->status != ENEMY_DEAD){
			if(p.pos.x + camera.pos.x > e->pos.x - 200 && p.pos.x+ camera.pos.x < e->pos.x - 50 ){
				if(e->pos.x >= e->posInit.x - e->maxrange && e->pos.x <=e->posInit.x + e->maxrange){//check if enemy is inside his max range
					if (e->pos.x - ((e->pos.x - p.pos.x+camera.pos.x)*dt*0.5)/1000.0 >= e->posInit.x - e->maxrange )
					{
						e->pos.x -= ((e->pos.x - p.pos.x+camera.pos.x)*dt*0.5)/1000.0;
						e->laststatus = e->status;
						e->status = ENEMY_LEFT;
						
					}
					else{
						e->laststatus = e->status;
						e->status = ENEMY_IDLE;
						
					}
					e->enemyReturnTimer=2000;
				}
				
			} 
			else if(p.pos.x + camera.pos.x< e->pos.x + 200 && p.pos.x+camera.pos.x > e->pos.x + 50)
			{
				if(e->pos.x >= e->posInit.x - e->maxrange && e->pos.x <= e->posInit.x + e->maxrange){//check if enemy is inside his max range
					if(e->pos.x +  ((p.pos.x + camera.pos.x - e->pos.x)*dt*0.5)/1000.0 <= e->posInit.x + e->maxrange){
						e->pos.x += ((p.pos.x + camera.pos.x - e->pos.x)*dt*0.5)/1000.0;
						e->laststatus = e->status;
						e->status = ENEMY_RIGHT;
					}
					else{
						e->laststatus = e->status;						
						e->status = ENEMY_IDLE;
					}
					e->enemyReturnTimer=2000;
				}
			} 
			else{
				if (!detect_collision(&p, e, camera))
				{
					
				if(e->enemyReturnTimer>0 && e->pos.x != e->posInit.x)	{
					e->laststatus = e->status;
					e->status = ENEMY_IDLE;					
					e->enemyReturnTimer-=dt;
				}
				if(e->enemyReturnTimer<=0){
					if(e->pos.x > e->posInit.x){
						e->pos.x -= ((e->pos.x - e->posInit.x)*dt*0.5)/1000.0;
						e->laststatus = e->status;
						e->status = ENEMY_LEFT;	
					}
					else if(e->pos.x < e->posInit.x){
						e->pos.x += ((e->posInit.x - e->pos.x)*dt*0.5)/1000.0;
						e->laststatus = e->status;
						e->status = ENEMY_RIGHT;
					}

					if (e->pos.x > e->posInit.x-60&& e->pos.x < e->posInit.x)
					{
						e->pos.x = e->posInit.x;
						e->enemyReturnTimer=2000;
						e->laststatus = ENEMY_LEFT;
						e->status = ENEMY_IDLE;
					}
					else if(e->pos.x < e->posInit.x+60 && e->pos.x > e->posInit.x)
					{
						e->laststatus = ENEMY_RIGHT;
						e->status = ENEMY_IDLE;
						e->pos.x = e->posInit.x;
						e->enemyReturnTimer=2000;
					}
					else if(e->pos.x == e->posInit.x){
						e->laststatus = e->status;
						e->status = ENEMY_IDLE;
						e->enemyReturnTimer=2000;
					
					}

				}
					
				
				}
			
			}
		}
		
	
	
		printf("\n\n\n\nENEMY RETURN TIMER: %d\n", e->enemyReturnTimer);
		printf("ENEMY POS X: %d | ENEMY POS INIT X: %d\n", e->pos.x, e->posInit.x);

}

void ai_enemy(Player *p, Enemy *e, Camera camera, Uint32 currentTime){
	if (e->status != ENEMY_DEAD){
		if(detect_collision(p, e, camera)){
			{
			e->isFighting=1;
			if(currentTime - e->lastdamagetime > 2000){
				e->punched=1;
				e->lastdamagetime = currentTime;
				e->frame=0;
				}
			}
			
		}
		else{
			e->isFighting=0;
			//e->frame=0;	
		}
			
	}

}


int check_enemy(Player *p, Enemy *e, Camera camera, Mix_Chunk *health_hit_sfx){
	if(e->status != ENEMY_DEAD )//&& detect_collision(p, e, camera))
	{
		printf("enemy and player colliding\n\n\n\n\n\n");
		if(p->isFighting && p->frame%2==0){
			//e->hp--;
			//if((p->status == PLAYER_RIGHT && e->status == ENEMY_LEFT ) || (p->status == PLAYER_LEFT && e->status == ENEMY_RIGHT )) {
				if(e->hp>0)
					e->hp--;
				e->hpbarprogress.w = (100 * e->hp)/12;
				Mix_PlayChannel(2,p->punch_sfx, 0);
			//}	
		}
		else if(!p->isGuarding && e->punched && e->frame==3){
			if(p->hp>0)			
				p->hp--;
			//e->isFighting=0;
			e->punched=0;
			//e->frame=0;
			Mix_PlayChannel(2,health_hit_sfx, 0);
		}
		else if(p->isGuarding && e->punched && e->frame==3){
			
			//e->isFighting=0;
			e->punched=0;
			//e->frame=0;
		}
		
		if(e->hp <=0 ){
			e->laststatus = e->status;
			e->status = ENEMY_DEAD;
			printf("ENEMY DEAD\n");
			return 1;
		}
	}	
	return 0;

}


void animer_enemy(Enemy *e,Uint32 currentTime, Uint32 dt){
	if(e->status!=ENEMY_DEAD && currentTime-e->lastframetime>150){
		if(!e->isFighting)
			e->frame = (e->frame+1)%4;
		else{//if enemy is fighting => punch player only once
			//if(e->frame<3)
				//e->frame++;
			if(e->punched)
				e->frame++;
			else
				e->frame=0;
		}
		e->lastframetime=currentTime;
		e->pos.w = e->spritesheets[0].img->w / 4;
		e->pos.h = e->spritesheets[0].img->h;
		e->lastframetime = currentTime;
	}
	else if(e->status==ENEMY_DEAD && currentTime-e->lastframetime>150){
		if(e->frame<4)
			e->frame++;
		e->lastframetime=currentTime;
		e->pos.w = e->spritesheets[2].img->w / 4;
		e->pos.h = e->spritesheets[2].img->h;
		e->lastframetime = currentTime;
	}


}

int detect_collision(Player *p, Enemy *e, Camera camera)
{
	//printf(" e pos width: %d | e pos height: %d\n", e->pos.w, e->pos.h);
	//printf("enemy frame: %d | enemy pos rect clip x: %d | enemy pos rect clip w: %d | pos x: %d | pos y: %d \n", e->frame, e->leftframes[e->frame][0].x, e->leftframes[e->frame][0].w, e->pos.x, e->pos.y);
	//printf(" player real pos: %d | player real  pos: %d\n", p->pos.x + p->playerW + camera.pos.x, p->pos.y + p->playerH + camera.pos.y);
    if ((p->pos.x + p->playerW + camera.pos.x  < e->pos.x ) || (p->pos.x  + camera.pos.x> e->pos.x + e->pos.w ) ||(p->pos.y + p->playerH  + camera.pos.y - 500 < e->pos.y) || (p->pos.y  + camera.pos.y - 500> e->pos.y + e->pos.h )){
        return 0;       
        }
    else
        return 1;
        
        
}



void liberer_enemy(Enemy e){
	for(int i=0;i<6;i++)
		liberer_image(e.spritesheets[i]);

}


































