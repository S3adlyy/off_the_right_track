#include "gameObject.h"



void init_gameObject(GameObject *obj, int x, int y, char filename[], int job){
	init_image(&obj->img, filename, x, y);
	for(int i=0;i<3;i++){
		obj->clip[i].x = 0;
		obj->clip[i].y = i * obj->img.img->h / 3;
		obj->clip[i].w = obj->img.img->w;
		obj->clip[i].h = obj->img.img->h / 3;
		
	
	}
	obj->job = job;
	obj->active=1;
	obj->frame=0;
	obj->sfx=NULL;
	obj->sfx =  init_chunk("game/gameobject.wav");
	if(obj->sfx==NULL){
		printf("ERROR SFX GAMEOBJECT\n");
		return;	
	}

}


void animate_gameObjects(GameObject *array, int n, Uint32 currenttime){
	for(int i=0;i<n;i++){
		if(currenttime-array[i].lastframetime > 200){
			array[i].frame = (array[i].frame+1)%3;
			array[i].lastframetime = currenttime;
			//printf("gameobject %d frame: %d\n", i, array[i].frame);
		}
	}

}

int update_gameObject(GameObject *array, int *n, Player p, Camera camera){

	for(int i=0;i<*n;i++){
		if(checkCollisionGO(p, array[i].img.pos1, camera)){
			Mix_PlayChannel(2,array[i].sfx, 0);
			int job = array[i].job;
			delete_gameObject(array, n, i);
			return job;//player cant collide with more than 1 game object at each time
		}
	
	}
	return -1;

}
void display_gameObjects(GameObject *array, int n, Camera camera, SDL_Surface *ecran, Uint32 currenttime){
	for(int i=0;i<n;i++){
	//array[i].img.pos1.x -= camera.pos.x;
	//array[i].img.pos1.y -= camera.pos.y - 500 ;//500 is init cam pos
	animate_gameObjects(array, n, currenttime);
	afficher_sprite_at_pos(array[i].img, ecran, &array[i].clip[array[i].frame], &(SDL_Rect){array[i].img.pos1.x - camera.pos.x,array[i].img.pos1.y - camera.pos.y + 500});
	}


}


void delete_gameObject(GameObject *array, int *n, int i) {
	if (i < 0 || i >= *n) {
		printf("Wrong position.\n");
		return;
	}

	// Shift elements to the left to cover the deleted element
	for (int j = i; j < (*n) - 1; j++) {
		array[j] = array[j + 1];
	}

	(*n)--;


	GameObject *temp = realloc(array, (*n) * sizeof(GameObject));
	if (temp == NULL) {
		printf("memory reallocation error.\n");
		return;
	}
	array = temp;
}


int checkCollisionGO(Player p, SDL_Rect b, Camera camera) {
	//printf("b x; %d | b y: %d | bw: %d | bh: %d\n", b.x, b.y, b.w, b.h/3);
	if (p.pos.x + p.playerW + camera.pos.x < b.x || // Player's right edge is to the left of the block
		p.pos.x + camera.pos.x > b.x + b.w ||       // Player's left edge is to the right of the block
		p.pos.y + p.playerH + camera.pos.y - 500 < b.y || // Player's bottom edge is above the block
		p.pos.y + camera.pos.y - 500> b.y + b.h/3) 
	{     
		return 0; // No collision
	} else {
		return 1; // Collision detected
	}

}

void liberer_gameObjects(GameObject *array, int n){
	for(int i=0;i<n;i++){
		liberer_image(array[i].img);
		liberer_chunk(array[i].sfx);
	
	}
	free(array);

}



