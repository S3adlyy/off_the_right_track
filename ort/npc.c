#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "fonctions.h"
#include "constants.h"
#include "npc.h"
#include "dialogue.h"
#include <string.h>




Npc init_npc(char filepath[], int job ,float x, float y, int npc_num){
	Npc npc;
	init_image(&npc.npc_img, filepath, x, y);//init player sprite
	for(int j=0;j<2;j++){
			npc.clip[j].x = j * npc.npc_img.img->w / 2;
			npc.clip[j].y = 0;
			npc.clip[j].w = npc.npc_img.img->w / 2;
			npc.clip[j].h = npc.npc_img.img->h;
		}
	npc.frame=0;
	init_image(&npc.talk_img, "game/talk_img.png", npc.npc_img.pos1.x+10, npc.npc_img.pos1.y-35);//init talk image above(E key) 
	npc.frames = 0;
	npc.job = job;
	switch(job){
		case 1:
		case 2:
		case 3:
		case 4:
			init_image(&npc.indicator_img, "game/npcenigmaindicator.png", npc.npc_img.pos1.x, npc.npc_img.pos1.y-50);
			break;
		case 5:
		case 6:
			init_image(&npc.indicator_img, "game/npcinfoindicator.png", npc.npc_img.pos1.x, npc.npc_img.pos1.y-50);
			break;
	}
	npc.job_completed=0;
	npc.currentDialogue = 1;
	npc.dialogueActive=0;
	npc.playerInProximity = 1;//CHANGE BACK TO 0
	npc.isActive=0;
	
	npc.nD2 = 2;
	switch(npc_num){
		case 1:
			initDialogue1x1(npc.Tdialogue1);
			initDialogue1x2(npc.Tdialogue2);
			npc.nD1 = 8;//n dialogue1 lines
			break;
		case 2:
			initDialogue2x1(npc.Tdialogue1);
			initDialogue2x2(npc.Tdialogue2);
			npc.nD1 = 7;//n dialogue1 lines
			break;
		case 3:
			npc.nD1=8;
			initDialogue3x1(npc.Tdialogue1);
			initDialogue3x2(npc.Tdialogue2);
			break;
		/*case 4:
			initDialogue4x1(npc.Tdialogue1);
			initDialogue4x2(npc.Tdialogue2);
			break;
		case 5:
			initDialogue5x1(npc.Tdialogue1);
			initDialogue5x2(npc.Tdialogue2);
			break;
		case 6:
			initDialogue6x1(npc.Tdialogue1);
			initDialogue6x2(npc.Tdialogue2);
			break;
		case 7:
			initDialogue7x1(npc.Tdialogue1);
			initDialogue7x2(npc.Tdialogue2);
			break;
		default:
			initDialogue6x1(npc.Tdialogue1);
			initDialogue6x2(npc.Tdialogue2);*/
			
	
	
	}
	
	
	return npc;
	
}


void display_npc(Npc npc, SDL_Surface *ecran, Camera camera){
	npc.npc_img.pos1.x -= camera.pos.x;
	npc.npc_img.pos1.y -= camera.pos.y - 500 ;//500 is init cam pos
	npc.indicator_img.pos1.x -= camera.pos.x;
	npc.indicator_img.pos1.y -= camera.pos.y - 500 ;//500 is init cam pos
	afficher_sprite(npc.npc_img, ecran, &npc.clip[npc.frame]);//change 3 to frames number
	if(!npc.isActive && !npc.job_completed && !npc.playerInProximity)
		afficher_image(npc.indicator_img, ecran);//affiche npc indicator
	if(!npc.isActive && npc.playerInProximity){
		npc.talk_img.pos1.x -= camera.pos.x;
		npc.talk_img.pos1.y -= camera.pos.y - 500;	
		afficher_image(npc.talk_img, ecran);
	}
}

void displayNpcDialogue(Npc *npc, SDL_Surface *ecran){
	if(npc->currentDialogue==1 && npc->dialogueActive){//change 8 to nD1
		activateDialogueMultiple(npc->Tdialogue1, npc->nD1);
		
		displayDialogueMultiple(ecran, npc->Tdialogue1, npc->nD1, &npc->dialogueActive);
	}
	else if(npc->currentDialogue==2 && npc->dialogueActive){//change 2 to nD2
		activateDialogueMultiple(npc->Tdialogue2, 2);
		displayDialogueMultiple(ecran, npc->Tdialogue2, 2, &npc->dialogueActive);
	}

}

int npcOnInteraction(Npc *npc, SDL_Surface *ecran, SDL_Event event)//activate npc dialogue and set activity to true
{
	printf("player in prox: %d || dialogue active : %d \n", npc->playerInProximity, npc->dialogueActive);
	if(npc->playerInProximity && !npc->dialogueActive ){
		npc->isActive = 1;
		npc->dialogueActive=1;
		npc->frame=1;
		//activateDialogueMultiple(npc->Tdialogue1, 8);
		displayNpcDialogue(npc, ecran);
		npc->dialogueActive=0;
		npc->currentDialogue=2;	
		npc->frame=0;
		printf("dialogue active: %d ", npc->dialogueActive);	
	}
	if(!npc->job_completed){
		return npc->job;
	}
	npc->isActive=0;
	return 0;//there is no job to be executed

}



void checkPlayerInProximity(Npc *npc, Player p, Camera camera){
	//include trigonometric collision here
	if(p.pos.x + camera.pos.x> npc->npc_img.pos1.x - 200 && p.pos.x + camera.pos.x < npc->npc_img.pos1.x+10 && p.pos.y + camera.pos.y - 500 > npc->npc_img.pos1.y - 100 && p.pos.y + camera.pos.y - 500 < npc->npc_img.pos1.y + npc->npc_img.pos1.h){//add Y control conditions
		npc->playerInProximity = 1;
		printf("PLAYER IN PROXIMITY | npc pos x: %d\n", npc->npc_img.pos1.x);
		npc->frame=1;
	}  
	else{
		npc->playerInProximity=0;
		npc->frame=0;
		}



}

void liberer_npc(Npc npc){
	liberer_image(npc.talk_img);
	liberer_image(npc.indicator_img);
	liberer_image(npc.npc_img);

}


//include trigonometric collision to check if player is in npc proximity, if yes -> if player clicks on E to interact with the npc -> a function called npcOnInteraction is called



























