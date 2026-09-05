#ifndef PLAYER_H
#define PLAYER_H

#include "types.h"

Rectangle getPlayerRect(GS* gs);
void playerMovement(GS* gs,anim* anim,float dt);
void Gravity(GS* gs,float dt);
void drawPlayerSprite(GS* gs);
void setplayerstate(GS* gs);
void updateJumpFrame(GS* gs);
void playerDashUpdate(GS* gs,float dt);
Rectangle getplayerhitbox(GS* gs);
void hitting(GS* gs,float dt);
void restrict_left_movement(GS* gs);
void checkHealthPickup(GS* gs);
float getPlayerCenterX(GS* gs);
#endif