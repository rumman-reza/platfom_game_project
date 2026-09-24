#ifndef GAME_H
#define GAME_H
#include"types.h"
#include"health.h"
void drawGame(GS* gs);
void initGame(GS* gs, tex* tex, anim* anim);
void updateGame(GS* gs,anim* anim, float dt);
void updateGameplay(GS* gs,anim* anim,float dt);
//menu
void updateMenu(GS* gs);
void drawMenu(GS* gs);
//game 
void updateNameEntry(GS* gs);
void drawNameEntry(GS* gs);
void unloadenemy(GS* gs);

#endif 