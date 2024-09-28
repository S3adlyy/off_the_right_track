#ifndef FONCTIONS_H
#define FONCTIONS_H
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <time.h>


typedef struct {
	SDL_Rect pos1;
	SDL_Rect pos2;
	SDL_Surface *img;
	
}Image;
typedef struct{
	SDL_Surface *txt;
	SDL_Rect pos_txt;
	SDL_Color color;
	TTF_Font *police;

}Text;

typedef struct{
	Image img; //change image position if cursor is hovered on the objective or not
	Text obj_txt, obj_n;//change objective color to green or red depending on if its done or not
	char txtcontent[40], txtcontent2[6];
	int collected_n,max_n;
	int done; //1- done | 0- not done

}Objective;

//image functions headers
void init_image(Image *A, char filepath[], float x, float y);
void size_image(Image *A, int width, int height);//width: to set the sprite sheets widths to the width of 1 sprite, height:same
void afficher_image(Image A, SDL_Surface *ecran);
void afficher_sprite(Image A, SDL_Surface* ecran, SDL_Rect* clip);
void afficher_sprite_at_pos(Image A, SDL_Surface* ecran, SDL_Rect* clip, SDL_Rect *pos);//display image with clip with custom pos
void liberer_image(Image A);
void changeImagePos(Image *A, SDL_Rect new_pos);

//text functions headers
Text init_text(float x, float y,char fontname[],int fontsize, int r, int g, int b);
void afficher_text(SDL_Surface *ecran, Text *txte, char textcontent[]);
void liberer_text(Text *txte);

//long audio functions headers
void init_music(Mix_Music *music, char musicpath[]);
void liberer_music(Mix_Music *music);
//long audio functions headers
Mix_Chunk* init_chunk(char chunkpath[]);
void liberer_chunk(Mix_Chunk *chunk);


//buttons and collisions and hovers and clicks
int button_hovered(SDL_Event event, Image btn);
int button_clicked(SDL_Event event, Image btn);//starting x,y are where starting point of the collision(image initialized position)
int text_hovered(SDL_Event event, Text btn);
int text_clicked(SDL_Event event, Text btn);

//objectives related func
void init_objective(Objective *obj, char content[], int max_n,float y);
void afficher_objective(Objective obj, SDL_Surface *ecran);
void update_objective(Objective *obj, int add_n);//adds (add_n) to collected_n and checks if objective is done or not
void animate_objective(Objective *obj, int obj_hovered);
void liberer_objective(Objective obj);


//cursor related funcs
SDL_Surface * init_cursor(SDL_Rect *cur_pos);
void updateCursorPosition(float *oldX, float *oldY, SDL_Rect *curPos, SDL_Event event);
void displayCursor(SDL_Surface *cursor, SDL_Rect cur_pos, float oldCurPosX, float oldCurPosY, SDL_Surface *ecran);

//settings file save
void saveSettings(int volumeMusic, int volumeSfx);
void loadSettings(int *volumeMusic, int *volumeSfx);

float randomFloat(float min, float max) ;
int random_int(int min, int max) ;
void HideCursor();
#endif
