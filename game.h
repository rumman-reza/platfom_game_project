#ifndef GAME_H
#define GAME_H
#include"types.h"

void drawGame(GS* gs);
void initGame(GS* gs, tex* tex, anim* anim);
void updateGame(GS* gs,anim* anim, float dt);
void updateGameplay(GS* gs,anim* anim,float dt);


#endif 