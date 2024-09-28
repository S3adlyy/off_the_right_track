#ifndef NPC_H
#define NPC_H
#include "dialogue.h"
#include "scrolling.h"

typedef struct{
	Image npc_img;
	Image indicator_img;
	Image talk_img;//interaction key
	SDL_Rect clip[2];
	int frame;
	int playerInProximity;//check if player is in proximity -> activate Talk option 
	int frames;
	int job;//0- finished 1-minigame ortcrush 2- tictactoe 3- Q&A 4- minigame memory card 5- INFORMATIONAL NPC(gives documents) 6- tutorial npc
	int job_completed;//1-yes 0- no  (related to game save system)
	Dialogue Tdialogue1[8];//dialogue at first interaction
	Dialogue Tdialogue2[8]; //dialogue at second (or +) interaction  
	int nD1, nD2;//dialogues length 
	int currentDialogue;//1 or 2 //-----RELATED TO GAME SAVE SYSTEM
	int dialogueActive;//for dialogue activation and desactivation
	int isActive; //1- npc is interacting with the player = active

}Npc;

Npc init_npc(char filepath[], int job, float x, float y, int npc_num);
void display_npc(Npc npc, SDL_Surface *ecran, Camera camera);
void displayNpcDialogue(Npc *npc, SDL_Surface *ecran);
void checkPlayerInProximity(Npc *npc, Player p, Camera camera);
int npcOnInteraction(Npc *npc, SDL_Surface *ecran, SDL_Event event);
void liberer_npc(Npc npc);



#endif /* NPC_H */

