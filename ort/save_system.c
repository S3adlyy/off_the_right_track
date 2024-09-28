#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "fonctions.h"
#include "constants.h"
#include "save_system.h"
#include "npc.h"
#include <string.h>







void saveGame(const char *filename, const GameData *data) {
    FILE *file = fopen(filename, "wb"); 
    
    if (file != NULL) {
        fwrite(data, sizeof(GameData), 1, file); // write  data struct to  file
        fclose(file); 
    } else {
        printf("Error: Unable to open file for writing.\n");
    }
}

int loadGame(const char *filename, GameData *data) {
    FILE *file = fopen(filename, "rb"); 
    
    if (file != NULL) {
        fread(data, sizeof(GameData), 1, file); // read data struct from file
        fclose(file); 
        return 1;
    } else {
        printf("Error: Unable to open file for reading.\n");
        return 0;
    }
}



int loadLevelData(int level, GameData *data) {
    char filename[20]; 
    sprintf(filename, "level%d.dat", level);
    
    // load game data for the specified level
    if(loadGame(filename, data))
    	return 1;
    return 0;
}

void saveLevelData(int level, GameData *data) {
    char filename[20];
    sprintf(filename, "level%d.dat", level);
    
    // save game data for the specified level
    saveGame(filename, data);
}



//a functions that reads data from a file if existant (hp, resources, level, position x:y, camera position, enemies killed?, etc..)
//a function that is called in every end of a level or whenever the user quits (before) => saves data into files
