#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "fonctions.h"
#include "constants.h"
#include "dialogue.h"
#include <string.h>
#define TEXT_TYPING_SPEED 30 // milliseconds per character


void displayDialogue(SDL_Surface *ecran, char *text, char *name, char *imagepath, int *dialogueActive) {
    Image dialoguebox, imagecharacter, skipKey;
    init_image(&dialoguebox, "game/dialoguebox.png",SCREEN_W*0.17,SCREEN_H*0.71);
    init_image(&imagecharacter, imagepath,dialoguebox.pos1.x+dialoguebox.pos1.w*0.02,dialoguebox.pos1.y+dialoguebox.pos1.h*0.08);
    Text txt_dialoguetxt= init_text(dialoguebox.pos1.x+dialoguebox.pos1.w*0.20, dialoguebox.pos1.y+dialoguebox.pos1.h*0.52, "fonts/emporia-roman.otf", 30, 255, 255, 255);
    Text txt_dialoguetxt2= init_text(dialoguebox.pos1.x+dialoguebox.pos1.w*0.20, dialoguebox.pos1.y+dialoguebox.pos1.h*0.62, "fonts/emporia-roman.otf", 30, 255, 255, 255);
    Text txt_dialoguetxt3= init_text(dialoguebox.pos1.x+dialoguebox.pos1.w*0.20, dialoguebox.pos1.y+dialoguebox.pos1.h*0.72, "fonts/emporia-roman.otf", 30, 255, 255, 255);
    Text txt_name= init_text(dialoguebox.pos1.x+dialoguebox.pos1.w*0.19, dialoguebox.pos1.y+dialoguebox.pos1.h*0.31, "fonts/emporia-roman.otf", 29, 255, 255, 255);
    init_image(&skipKey, "game/enterkey.png",dialoguebox.pos1.x+dialoguebox.pos1.w*0.96,dialoguebox.pos1.y+dialoguebox.pos1.h*0.80);//depends on the player controller mode(if k&m or controller)


    char* lines[3]; 
    int lineCount = 0;
    char textch[512];
    strcpy(textch, text);
    char* token = strtok(textch, "\n");
   while (token != NULL && lineCount < 3) {
        lines[lineCount++] = token;
        token = strtok(NULL, "\n");
    }
    int currentLine = 0;
    int currentChar = 0;
    int startTime = SDL_GetTicks();
    int typingFinished = 0, endText=0;

    while (!typingFinished) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                if (currentChar<strlen(lines[currentLine])) {
         	   currentLine=lineCount-1;
         	   currentChar = strlen(lines[lineCount-1]);
         	   endText=1;
                } else {
                    typingFinished=1;
                }
            }
        }

        afficher_image(dialoguebox, ecran);
        afficher_image(imagecharacter, ecran);
        afficher_image(skipKey, ecran);
        afficher_text(ecran, &txt_name, name);
        if (currentLine < lineCount && !typingFinished) {
		    char currentText[strlen(lines[currentLine]) + 1]; 
		    strncpy(currentText, lines[currentLine], currentChar+1);
		    currentText[currentChar+1] = '\0';
		    printf("current line:%d| line count: %d| currentchar: %d | currentText: %s\n", currentLine, lineCount, currentChar, currentText);
		    
            switch(currentLine){
         	case 0:
         	   afficher_text(ecran, &txt_dialoguetxt, currentText);break;
         	case 1:
         	   afficher_text(ecran, &txt_dialoguetxt, lines[0]);
         	   afficher_text(ecran, &txt_dialoguetxt2, currentText);break;
         	case 2:
         	   afficher_text(ecran, &txt_dialoguetxt, lines[0]);
         	   afficher_text(ecran, &txt_dialoguetxt2, lines[1]);
         	   afficher_text(ecran, &txt_dialoguetxt3, currentText);break;
		}
            if (SDL_GetTicks() - startTime >= TEXT_TYPING_SPEED && !endText) {
                startTime = SDL_GetTicks();
                currentChar++;
            }
	if(currentChar==strlen(lines[currentLine])){
		if(currentLine<lineCount-1){
		currentLine++;
		currentChar=0;}
		else
			endText=1;
	}
        }
        SDL_Flip(ecran);
    }

    *dialogueActive = 0;
    liberer_text(&txt_dialoguetxt);
    liberer_text(&txt_dialoguetxt2);
    liberer_text(&txt_dialoguetxt3);
     liberer_text(&txt_name);
    liberer_image(dialoguebox);
    liberer_image(skipKey);
    liberer_image(imagecharacter);
}


void displayDialogueMultiple(SDL_Surface *ecran, Dialogue T[], int n, int *dialogueActive)//n=number of dialogues to display from the table
{
	int i;
	for(i=0;i<n;i++){
		displayDialogue(ecran, T[i].text, T[i].name, T[i].imagepath, &T[i].active) ;
	}
	*dialogueActive=0;
}

void activateDialogueMultiple(Dialogue T[], int n){
	int i;
	for(i=0;i<n;i++){
	//for(i=0;T[i]!='\0';i++){
		T[i].active = 1;
	}
}

void initDialogue1(Dialogue Tdialogue[])
{
	Tdialogue[0].text = "Ah. Greetings traveler. What brings you to our humble village?";
        Tdialogue[0].name = "Shamir (Villager)";
        Tdialogue[0].imagepath = "game/npcimage.png";
        Tdialogue[1].text = "I seek knowledge and understanding, but it seems scarce in these lands. Do you know of any \npaths to enlightenment?";
        Tdialogue[1].name = "Zoe (You)";
        Tdialogue[1].imagepath = "game/Zoepic.png";
        Tdialogue[2].text = "Knowledge, you say? A rare and precious comodity indeed. Alas, in these times, such luxuries\n are reserved for the privileged few. ";
        Tdialogue[2].name = "Shamir (Villager)";
        Tdialogue[2].imagepath = "game/npcimage.png";
        Tdialogue[3].text = "But should not the pursuit of knowledge be a right for all, regardless of station or birth?";
        Tdialogue[3].name = "Zoe (You)";
        Tdialogue[3].imagepath = "game/Zoepic.png";
        Tdialogue[4].text = "A noble sentiment, traveler, but alas, the world does not always adhere to noble ideals. \nYet, perhaps there is hope. I have hidden within the folds of this village a puzzle, a challenge that,\n if solved, may grant you a glimpse of the enlightment you seek.";
        Tdialogue[4].name = "Shamir (Villager)";
        Tdialogue[4].imagepath = "game/npcimage.png";
        Tdialogue[5].text = "A challenge, you say? I am intrigued. Pray, reveal to me this challenge, and i shall \nendeavor to overcome it.";
        Tdialogue[5].name = "Zoe (You)";
        Tdialogue[5].imagepath = "game/Zoepic.png";
        Tdialogue[6].text = "Very well.";
        Tdialogue[6].name = "Shamir (Villager)";
        Tdialogue[6].imagepath = "game/npcimage.png";



}


void initDialogue1x1(Dialogue Tdialogue[])
{
	Tdialogue[0].text = "Ah. Greetings traveler. What brings you to these ancient sands?";
        Tdialogue[0].name = "Shamir (Villager)";
        Tdialogue[0].imagepath = "game/npcimage.png";
        Tdialogue[1].text = "I walk these paths in search of truth, yet the whispers of the past seem to shroud my vision.\n Is there wisdom to be found here?";
        Tdialogue[1].name = "Zoe (You)";
        Tdialogue[1].imagepath = "game/Zoepic.png";
        Tdialogue[2].text = "Wisdom, a gem rare and precious, but in this era, it is a privilege bestowed upon the \nchosen few. The air itself carries the weight of discrimination, and the shadows of segregation \nloom large.";
        Tdialogue[2].name = "Shamir (Villager)";
        Tdialogue[2].imagepath = "game/npcimage.png";
        Tdialogue[3].text = "But should not the light of fairness penetrate even the darkest corners of society,\n illuminating all with its radiance?";
        Tdialogue[3].name = "Zoe (You)";
        Tdialogue[3].imagepath = "game/Zoepic.png";
        Tdialogue[4].text = "A noble sentiment, traveler, but alas, the sands of ancient Egypt bore witness to a world\n where privilege was a currency traded only among the elite. The cries for equality were but\n whispers lost in the wind.";
        Tdialogue[4].name = "Shamir (Villager)";
        Tdialogue[4].imagepath = "game/npcimage.png";
        Tdialogue[5].text = "Is there no hope, then, amidst this sea of oppression?";
        Tdialogue[5].name = "Zoe (You)";
        Tdialogue[5].imagepath = "game/Zoepic.png";
        Tdialogue[6].text = "Hope, like a flame flickering in the wind, is a fragile thing in these lands. \nYet, within the heart of the pyramid, your might find an answer. Here is something that might\n help.";
        Tdialogue[6].name = "Shamir (Villager)";
        Tdialogue[6].imagepath = "game/npcimage.png";
        Tdialogue[7].text = "Very well. I shall heed your words.";
        Tdialogue[7].name = "Zoe (You)";
        Tdialogue[7].imagepath = "game/Zoepic.png";



}

void initDialogue1x2(Dialogue Tdialogue[])
{
        Tdialogue[0].text = "Beware, traveler, for the whispers among the villagers speak of restless spirits, \nof mummies stirring within the ancient tombs. Tread carefully, lest you awaken the wrath \nof the long-forgotten guardians of the pyramid.";
        Tdialogue[0].name = "Shamir (Villager)";
        Tdialogue[0].imagepath = "game/npcimage.png";

	Tdialogue[1].text = "Venture forth, brave soul, and may the sands of time reveal their secrets to you.";
        Tdialogue[1].name = "Shamir (Villager)";
        Tdialogue[1].imagepath = "game/npcimage.png";



}



void initDialogue2x1(Dialogue Tdialogue[])
{
	Tdialogue[0].text = "Traveler, have you heard the stories of the ancient tombs? \nThey say the pyramids are haunted by the spirits of the past.";
        Tdialogue[0].name = "Meri (Villager)";
        Tdialogue[0].imagepath = "game/npcimage.png";
        Tdialogue[1].text = "Haunted? By what, exactly?";
        Tdialogue[1].name = "Zoe (You)";
        Tdialogue[1].imagepath = "game/Zoepic.png";
        Tdialogue[2].text = "Mummies, they say. The villagers whisper of cursed\souls that guard the secrets\n buried within.";
        Tdialogue[2].name = "Meri (Villager)";
        Tdialogue[2].imagepath = "game/npcimage.png";
        Tdialogue[3].text = "That sounds dangerous. Why would anyone venture inside?";
        Tdialogue[3].name = "Zoe (You)";
        Tdialogue[3].imagepath = "game/Zoepic.png";
        Tdialogue[4].text = "For some, it is the promise of untold treasures. \nFor others, the hope of uncovering lost knowledge.\n But be warned, the path is fraught with peril.";
        Tdialogue[4].name = "Meri (Villager)";
        Tdialogue[4].imagepath = "game/npcimage.png";
        Tdialogue[5].text = "I see. I must tread carefully then. Thank you for the warning";
        Tdialogue[5].name = "Zoe (You)";
        Tdialogue[5].imagepath = "game/Zoepic.png";
        Tdialogue[6].text = "May the gods protect you on your journey. \nHere is something that might help.";
        Tdialogue[6].name = "Meri (Villager)";
        Tdialogue[6].imagepath = "game/npcimage.png";



}

void initDialogue2x2(Dialogue Tdialogue[])
{
        Tdialogue[0].text = "You will find people like me along the way who will offer you a helping hand.";
        Tdialogue[0].name = "Meri (Villager)";
        Tdialogue[0].imagepath = "game/npcimage.png";

	Tdialogue[1].text = "Seek the truth, and it shall be brought to light.";
        Tdialogue[1].name = "Meri (Villager)";
        Tdialogue[1].imagepath = "game/npcimage.png";



}



void initDialogue3x1(Dialogue Tdialogue[])
{
	Tdialogue[0].text = "Ah, traveler. Do you see the weary faces in the fields? \nThey are the slaves, bound by chains of fate and toil.";
        Tdialogue[0].name = "Hasher (Villager)";
        Tdialogue[0].imagepath = "game/npcimage.png";
        Tdialogue[1].text = "Slaves? How can this be? Such cruelty and injustice...";
        Tdialogue[1].name = "Zoe (You)";
        Tdialogue[1].imagepath = "game/Zoepic.png";
        Tdialogue[2].text = "It is the way of our world. The powerful hold dominion, and the weak suffer \nunder their yoke. There is little hope for change.";
        Tdialogue[2].name = "Hasher (Villager)";
        Tdialogue[2].imagepath = "game/npcimage.png";
        Tdialogue[3].text = "But surely, every person deserves dignity and freedom.";
        Tdialogue[3].name = "Zoe (You)";
        Tdialogue[3].imagepath = "game/Zoepic.png";
        Tdialogue[4].text = "A noble thought, but here, dreams of equality are crushed beneath the weight\n of stone and the lash of the overseer.";
        Tdialogue[4].name = "Hasher (Villager)";
        Tdialogue[4].imagepath = "game/npcimage.png";
        Tdialogue[5].text = "Is there no way to challenge this?";
        Tdialogue[5].name = "Zoe (You)";
        Tdialogue[5].imagepath = "game/Zoepic.png";
        Tdialogue[6].text = "Some believe there are secrets hidden within the pyramid walls, truths that \ncould shatter the chains of oppression. But it is a dangerous path,\n filled with traps and guarded by the past.";
        Tdialogue[6].name = "Hasher (Villager)";
        Tdialogue[6].imagepath = "game/npcimage.png";
          Tdialogue[7].text = "Take this document. It contains what little we know. May it guide you in your quest.";
        Tdialogue[7].name = "Hasher (Villager)";
        Tdialogue[7].imagepath = "game/npcimage.png";



}

void initDialogue3x2(Dialogue Tdialogue[])
{
        Tdialogue[0].text = "Beware of what could be an obstacle to this enlightening path of yours.";
        Tdialogue[0].name = "Hasher (Villager)";
        Tdialogue[0].imagepath = "game/npcimage.png";

	Tdialogue[1].text = "You will find your way.";
        Tdialogue[1].name = "Hasher (Villager)";
        Tdialogue[1].imagepath = "game/npcimage.png";



}






/*void displayDialogue(SDL_Surface *ecran, char *text, char *name, char *imagepath, int *dialogueActive) {
    Image dialoguebox, imagecharacter, skipKey;
    init_image(&dialoguebox, "game/dialoguebox.png",SCREEN_W*0.17,SCREEN_H*0.71);
    init_image(&imagecharacter, imagepath,dialoguebox.pos1.x+dialoguebox.pos1.w*0.02,dialoguebox.pos1.y+dialoguebox.pos1.h*0.08);
    Text txt_dialoguetxt= init_text(dialoguebox.pos1.x+dialoguebox.pos1.w*0.20, dialoguebox.pos1.y+dialoguebox.pos1.h*0.52, "fonts/emporia-roman.otf", 30, 255, 255, 255);
    Text txt_dialoguetxt2= init_text(dialoguebox.pos1.x+dialoguebox.pos1.w*0.20, dialoguebox.pos1.y+dialoguebox.pos1.h*0.62, "fonts/emporia-roman.otf", 30, 255, 255, 255);
    Text txt_dialoguetxt3= init_text(dialoguebox.pos1.x+dialoguebox.pos1.w*0.20, dialoguebox.pos1.y+dialoguebox.pos1.h*0.72, "fonts/emporia-roman.otf", 30, 255, 255, 255);
    Text txt_name= init_text(dialoguebox.pos1.x+dialoguebox.pos1.w*0.19, dialoguebox.pos1.y+dialoguebox.pos1.h*0.31, "fonts/emporia-roman.otf", 26, 255, 255, 255);
    init_image(&skipKey, "game/enterkey.png",dialoguebox.pos1.x+dialoguebox.pos1.w*0.96,dialoguebox.pos1.y+dialoguebox.pos1.h*0.86);//depends on the player controller mode(if k&m or controller)
    char* line1 = strtok((char*)text, "\n");
    char* line2 = strtok(NULL, "\n");
    char* line3 = strtok(NULL, "\n");
    int line=1;
    int textLength = strlen(text);
    int currentChar = 0;
    int startTime = SDL_GetTicks();
    int typingFinished = 0;
    if (text == NULL) {
    // Handle NULL text
    	printf("Error: Text is NULL\n");
    	return;
	}	

    while (!typingFinished) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN){
		    if (event.key.keysym.sym == SDLK_RETURN) {
		    	printf("pressed return\n");
		    	if(currentChar==textLength)
		    		typingFinished=1;
		    	else
		        	currentChar = textLength;
		    }
        }
        }

        afficher_image(dialoguebox, ecran);
        afficher_image(imagecharacter, ecran);
	afficher_image(skipKey, ecran);
	afficher_text(ecran, &txt_name, name);
        if (currentChar < textLength) {
		    char currentText[currentChar + 1];  // +1 to include space for '\0'
		    strncpy(currentText, text, currentChar+1);
		    currentText[currentChar+1] = '\0';
		    if(currentText[currentChar]=='\n'){
		    	printf("line break;\n");
		    	line++;
		    }	

		    if (SDL_GetTicks() - startTime >= TEXT_TYPING_SPEED) {
			startTime = SDL_GetTicks();
			currentChar++;
		    }
		}
	 else {			
             	afficher_text(ecran, &txt_dialoguetxt, text);
       	 }
       	
        SDL_Flip(ecran);
    }
    printf("exited while loop");
    *dialogueActive=0;
    liberer_text(&txt_dialoguetxt);
    liberer_image(dialoguebox);
    liberer_image(skipKey);
    liberer_image(imagecharacter);
}

*/


