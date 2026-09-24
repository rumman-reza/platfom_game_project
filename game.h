#ifndef GAME_H
#define GAME_H
#include"types.h"
#include"health.h"
void drawGame(GS *gs,tex *textures);
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
void updateGameover(GS* gs);
void player_has_fallen(GS* gs);
void isGameover(GS* gs,float dt);
void drawGameover(GS* gs);
void updatescore(GS* gs);
void drawScoreHUD(const GS* gs);

void restartGame(GS* gs);


#endif 