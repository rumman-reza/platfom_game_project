#ifndef HEALTH_H
#define HEALTH_H
#include"types.h"
#include"player.h"

void updateHealth(GS* gs, float dt);
void damagePlayer(GS* gs,float amount);
void damageenemy(Enemy* enemy,float amount);

#endif