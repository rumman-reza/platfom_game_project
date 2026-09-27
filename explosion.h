#ifndef EXPLOSION_H
#define EXPLOSION_H
#include "types.h"

void spawnExplosion(GS* gs, Vector2 position);
void updateExplosions(GS* gs, float dt);
void drawExplosions(GS* gs, tex* textures);

#endif