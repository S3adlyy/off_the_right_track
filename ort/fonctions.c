#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "fonctions.h"
#include "constants.h"
#include <string.h>


//image related functions
void init_image(Image *A, char filepath[], float x, float y)
{
	SDL_Surface *img_converted=NULL;
	int x2 = (int)x;
	int y2 = (int)y;
	A->img = NULL;
	A->img = IMG_Load(filepath);
	if (A->img== NULL) {
		printf("Image load error: %s \n", IMG_GetError());
		return;
	}
	
	//convert image to screen format (more optimized)(test it more)
	/*img_converted =SDL_DisplayFormat(A->img); 
	SDL_FreeSurface(A->img);
	A->img = img_converted;*/
	A->pos1.x=x2;
	A->pos1.y=y2;
	A->pos1.w = A->img->w;
	A->pos1.h = A->img->h;

	A->pos2.x=0;
	A->pos2.y=0;
	A->pos2.w = A->img->w;
	A->pos2.h = A->img->h;
}

void size_image(Image *A, int width, int height){
	A->pos1.w = width;
	A->pos1.h = height;
}

void changeImagePos(Image *A, SDL_Rect new_pos){
	A->pos1.x = new_pos.x-40;
	A->pos1.y = new_pos.y-10;
}

void afficher_image(Image A, SDL_Surface *ecran){
	SDL_BlitSurface(A.img, NULL, ecran,  &A.pos1);
}

void afficher_sprite_at_pos(Image A, SDL_Surface* ecran, SDL_Rect* clip, SDL_Rect *pos){
	SDL_BlitSurface(A.img, clip, ecran, pos);
}
void liberer_image(Image A){
	SDL_FreeSurface(A.img);

}
//functiosn related to sprite sheets and image clipping display

void afficher_sprite(Image A, SDL_Surface* ecran, SDL_Rect* clip){
	//printf("x: %d \t y: %d\n", clip->x, clip->y);
	if(SDL_BlitSurface(A.img, clip, ecran, &A.pos1)<0){
		printf("%s\n", SDL_GetError());
	}
}

//text related functions

Text init_text(float x, float y,char fontname[],int fontsize, int r, int g, int b)
{
	Text txte;
	txte.txt=NULL;
	txte.police = TTF_OpenFont(fontname, fontsize);
	txte.color.r=r;
	txte.color.g=g;
	txte.color.b=b;
	txte.pos_txt.x = x;
	txte.pos_txt.y = y;
	return txte;
}

void afficher_text(SDL_Surface *ecran, Text *txte, char textcontent[])
{
	if (txte->police == NULL) {
        fprintf(stderr, "Font is not loaded: %s\n", TTF_GetError());
        return;
    	}
	txte->txt = TTF_RenderText_Blended(txte->police, textcontent, txte->color);
	
	if (txte->txt == NULL) {
        fprintf(stderr, "Unable to render text: %s\n", TTF_GetError());
        return;
    	}
	txte->pos_txt.w = txte->txt->w;
	txte->pos_txt.h = txte->txt->h;
       	SDL_BlitSurface(txte->txt, NULL, ecran, &txte->pos_txt);
}



void liberer_text(Text* txte){
	SDL_FreeSurface(txte->txt);
	if(txte->police!=NULL){
		TTF_CloseFont(txte->police);
		txte->police=NULL;}
        	
}


void init_music(Mix_Music *music, char musicpath[]){
	music = Mix_LoadMUS(musicpath);
	if(music==NULL)
	{
		printf("Error loading audio: %s", Mix_GetError());
		return ;
	}
	Mix_VolumeMusic((int)(0.6 * MIX_MAX_VOLUME));
	Mix_PlayMusic(music, -1);
}

void liberer_music(Mix_Music *music){
	Mix_FreeMusic(music);
}
//long audio functions headers
Mix_Chunk* init_chunk(char chunkpath[])//returns loaded chunk
{
	Mix_Chunk *chunk=NULL;
	chunk = Mix_LoadWAV(chunkpath);
	if(chunk==NULL)
	{
		printf("Error loading chunk: %s", Mix_GetError());
		return NULL;
	}
	return chunk;
	

}
void liberer_chunk(Mix_Chunk *chunk){
	Mix_FreeChunk(chunk);
}

//buttons and collisions

int button_hovered(SDL_Event event, Image btn){
	if(event.motion.y>=btn.pos1.y && event.motion.y<=btn.pos1.y+btn.pos1.h && event.motion.x>=btn.pos1.x && event.motion.x<=btn.pos1.x+btn.pos1.w)
		return 1;
	else
		return 0;
}
int button_clicked(SDL_Event event, Image btn){
	if(event.button.button == SDL_BUTTON_LEFT && 
	(event.motion.y>=btn.pos1.y && event.motion.y<=btn.pos1.y+btn.pos1.h  && event.motion.x>=btn.pos1.x && event.motion.x<=btn.pos1.x+btn.pos1.w))
	{
		return 1;
	}
	else
		return 0;
	
}


int text_hovered(SDL_Event event, Text btn){
	if(event.motion.y>=btn.pos_txt.y && event.motion.y<=btn.pos_txt.y+btn.pos_txt.h  && event.motion.x>=btn.pos_txt.x && event.motion.x<=btn.pos_txt.x+btn.pos_txt.w)
	{
		return 1;
	}
	else
		return 0;
	
}

int text_clicked(SDL_Event event, Text btn){
	if(event.button.button == SDL_BUTTON_LEFT && 
	(event.motion.y>=btn.pos_txt.y && event.motion.y<=btn.pos_txt.y+btn.pos_txt.h  && event.motion.x>=btn.pos_txt.x && event.motion.x<=btn.pos_txt.x+btn.pos_txt.w))
	{
		return 1;
	}
	else
		return 0;
	
}

SDL_Surface * init_cursor(SDL_Rect *cur_pos){
	SDL_Surface *cursorImage = NULL;
	cursorImage = IMG_Load("game/cursor32.png");
	if(cursorImage==NULL){
		printf("echec init cursor: %s\n", IMG_GetError());
		return NULL;
	}
	cur_pos->x = SCREEN_W/2;
	cur_pos->y = SCREEN_H/2;
	cur_pos->w = cursorImage->w;
	cur_pos->h= cursorImage->h; 
	return cursorImage;
}

void updateCursorPosition(float *oldX, float *oldY, SDL_Rect *curPos, SDL_Event event){

	*oldX = curPos->x;
	*oldY = curPos->y;
	curPos->x = event.motion.x;
	curPos->y= event.motion.y;
}

void displayCursor(SDL_Surface *cursor, SDL_Rect cur_pos, float oldCurPosX, float oldCurPosY, SDL_Surface *ecran){
	if(oldCurPosX!=cur_pos.x || oldCurPosY!=cur_pos.y){
		SDL_BlitSurface(cursor, NULL, ecran, &cur_pos);
	}
}

void saveSettings(int volumeMusic, int volumeSfx) {//note: try to hide the file from the user
	if (rename(".settings.txt", "settings.txt")!= 0) {
        fprintf(stderr, "Error renaming file | non existant\n");
    	} //unhide l fichier
	FILE *file = fopen("settings.txt", "w");
	if (file != NULL) {
		fprintf(file, "%d %d", volumeMusic, volumeSfx);
		fclose(file);
	} 
	else {
		printf("Didnt save settings.\n");
	}
	rename("settings.txt", ".settings.txt"); //hide l fichier
}


void loadSettings(int *volumeMusic, int *volumeSfx) {
	*volumeMusic = 125; 
	*volumeSfx=125; //defaut
	if (rename(".settings.txt", "settings.txt")!= 0) {
        fprintf(stderr, "Error renaming file | non existant\n");
    	} //unhide
	FILE *file = fopen("settings.txt", "r");
	if (file != NULL) {
		fscanf(file, "%d %d", volumeMusic, volumeSfx);
		fclose(file);
	}
	rename("settings.txt", ".settings.txt"); //hide
}

//objectives

void init_objective(Objective *obj, char content[], int max_n,float y)
{
	init_image(&(obj->img), "game/objective_img.png",0 , y);
	obj->img.pos1.x = obj->img.img->w * -0.75;
	obj->obj_txt = init_text(obj->img.pos1.x+obj->img.pos1.x*-0.1, 0.015*y+y, "fonts/emporia-roman.otf", 40, 255, 255, 255); 
	obj->obj_n = init_text(obj->img.img->w*0.80+obj->img.pos1.x,y+ 0.03*y, "fonts/emporia-roman.otf", 40, 255, 255, 255); 
	obj->done = 0;
	strcpy(obj->txtcontent, content);
	obj->max_n = max_n;
	obj->collected_n = 0;
}
void afficher_objective(Objective obj, SDL_Surface *ecran){
	afficher_image(obj.img, ecran);
	afficher_text(ecran, &obj.obj_txt, obj.txtcontent);
	sprintf(obj.txtcontent2, "%d/%d", obj.collected_n, obj.max_n);
	afficher_text(ecran, &obj.obj_n, obj.txtcontent2);
}

void animate_objective(Objective *obj, int obj_hovered){
	if(obj_hovered){
		if(obj->img.pos1.x < 0){
			if(obj->img.pos1.x + 35>0){
				obj->img.pos1.x += 0 - obj->img.pos1.x;	
			}
			else{obj->img.pos1.x += 35;
			obj->obj_txt.pos_txt.x += 35;
			obj->obj_n.pos_txt.x += 35;}
		}
	}
	else
	{
		if(obj->img.pos1.x > (obj->img.img->w * -0.75) && !obj_hovered){
			if(obj->img.pos1.x - 35 < (obj->img.img->w * -0.75)){
				obj->img.pos1.x -= obj->img.pos1.x - (obj->img.img->w * -0.75) ;
				}
			else{
				obj->img.pos1.x -= 35;
				obj->obj_txt.pos_txt.x -= 35;
				obj->obj_n.pos_txt.x -= 35;
			}	
		}
	}


}

void liberer_objective(Objective obj){
	liberer_image(obj.img);
	liberer_text(&obj.obj_txt);
	liberer_text(&obj.obj_n);

}

void update_objective(Objective *obj, int add_n){
	obj->collected_n += add_n;
	if(obj->collected_n>= obj->max_n){
		obj->done =1;
		//change to green
		obj->obj_n.color.r=0;
		obj->obj_n.color.g=128;
		obj->obj_n.color.b=0;
		}
	
}





//pause menu

//void PauseMenu(SDL_Surface *ecran, int *pausemenuActive, SDL_Surface *cursorImage4,SDL_Rect cur_pos, float oldCurPosX, float oldCurPosY, )




float randomFloat(float min, float max) {
    return min + (float)rand() / RAND_MAX * (max - min);
}

int random_int(int min, int max) {
    // Seed the random number generator with the current time
    srand(time(NULL));
    
    // Generate a random number between min and max
    return min + rand() % (max - min + 1);
}
void HideCursor()
{
	Uint8 l_data[1];
	Uint8 l_mask[1];

	l_data[0] = 0;
	l_mask[0] = 1;

	SDL_Cursor *g_cursor = SDL_CreateCursor(l_data, l_mask , 1, 1, 0,0);
	SDL_SetCursor(g_cursor);
}//to hide cursor-better than sdl set cursor 0












