#ifndef DIALOGUE_H
#define DIALOGUE_H
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>


typedef struct{
	char *text, *name, *imagepath;
	int active;

}Dialogue;

void displayDialogue(SDL_Surface *ecran, char *text, char *name, char *imagepath, int *dialogueActive) ;//text=dialogue text | name=dialogue speaker | imagepath=dialogue speaker character image path
void activateDialogueMultiple(Dialogue T[], int n);
void displayDialogueMultiple(SDL_Surface *ecran, Dialogue T[], int n, int *dialogueActive);
//dialogues tables initialization
void initDialogue1(Dialogue Tdialogue[]);

//NPC X DIALOGUES
void initDialogue1x1(Dialogue Tdialogue[]);
void initDialogue1x2(Dialogue Tdialogue[]);
void initDialogue2x1(Dialogue Tdialogue[]);
void initDialogue2x2(Dialogue Tdialogue[]);
void initDialogue3x1(Dialogue Tdialogue[]);
void initDialogue3x2(Dialogue Tdialogue[]);




#endif
