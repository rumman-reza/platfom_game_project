#ifndef PLAYER_H
#define PLAYER_H

#include "types.h"

Rectangle getPlayerRect(GS* gs);
void playerMovement(GS* gs,anim* anim,float dt);
void Gravity(GS* gs,float dt);
void drawPlayerSprite(GS* gs);
void setAnimation(GS* gs,anim_name name);
void updateJumpFrame(GS* gs);
void playerDashUpdate(GS* gs,float dt);
Rectangle gethitbox(GS* gs);
void hitting(GS* gs,float dt);
void restrict_left_movement(GS* gs);
void updateHealth(GS* gs, float dt);


#endif