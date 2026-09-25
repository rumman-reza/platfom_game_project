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
void damageEnemy(GS* gs,Enemy* enemy,float amount);
void updateEnemyAnimations(GS* gs,float dt);
void updateEnemyInvultimer(GS* gs,float dt);
void move_pgas(GS* gs,float dt);
Rectangle getPgasRect(GS* gs);
void drawPgasSprite(GS* gs);
void spawnEnemy(GS* gs, float x, float groundY);

//traps and stuff

void DamageFromSpikes(GS* gs,float dt);
#endif