#ifndef SAVE_SYSTEM_H
#define SAVE_SYSTEM_H
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "fonctions.h"
#include "constants.h"
#include "npc.h"
#include <string.h>


typedef struct {
    int level;
    int playerX;
    int playerY;
    int cameraX, cameraY;
    int playerHP;
    Npc Tnpc[8];//fill this array with spawned npcs when saving
    int n_npc;//number of npcs
    // add more variables for minigames, objectives, enemies, etc..
} GameData;

void saveGame(const char *filename, const GameData *data);
int loadGame(const char *filename, GameData *data);
int loadLevelData(int level, GameData *data);
void saveLevelData(int level, GameData *data);




#endif /* SAVE_SYSTEM_H */

