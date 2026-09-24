#ifndef HEALTH_H
#define HEALTH_H
#include"types.h"
#include"player.h"

//function er nam health.h h holeo sob header feature ekhane 
//implement kora hocche 
void updateHealth(GS* gs, float dt);
void damagePlayer(GS* gs,float amount);
// void damageEnemy(Enemy* enemy,float amount);
void drawHealthUI(GS* gs);   //health index er jonno UI
void updatePlayerInvulnerability(GS* gs,float dt);

#endif