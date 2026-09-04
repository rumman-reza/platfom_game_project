#ifndef ENEMY_H
#define ENEMY_H
#include "types.h"

Rectangle getEnemyRect(Enemy* enemy);
void updateEnemy(GS* gs,float dt);
Enemy loadEnemy(tex* tex);
void drawEnemy(Enemy* a);
void UnloadEnemyAnims(Enemy* enemy);
void updateEnemyAnimation(Enemy* enemy,enemy_anim en_anim);
Rectangle getEnemyHitbox(Enemy* enemy);
void damageEnemy(Enemy* enemy,float amount);
void updateEnemyAnimations(GS* gs,float dt);
void updateEnemyInvultimer(GS* gs,float dt);
#endif