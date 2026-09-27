#include "explosion.h"

void spawnExplosion(GS* gs, Vector2 position){
    int index = gs->explosion_index;

    gs->explosions[index].position = position;
    gs->explosions[index].timer = 0.0f;
    gs->explosions[index].currentframe = 0;
    gs->explosions[index].active = true;

    gs->explosion_index = (gs->explosion_index + 1) % max_explosions;
}

void updateExplosions(GS* gs, float dt){
    for(int i=0;i<max_explosions;i++){
        Explosion* e = &gs->explosions[i];
        if(!e->active) continue;

        e->timer += dt;
        if(e->timer >= explosion_frameduration){
            e->timer -= explosion_frameduration;
            e->currentframe++;
            if(e->currentframe >= explosion_framecount){
                e->active = false;
            }
        }
    }
}

void drawExplosions(GS* gs, tex* textures){
    float frameW = textures->bomb_explosion.width  / (float)explosion_framecount;
    float frameH = textures->bomb_explosion.height;
    float drawW = frameW * bomb_explosion_scale;
    float drawH = frameH * bomb_explosion_scale;

    for(int i=0;i<max_explosions;i++){
        Explosion* e = &gs->explosions[i];
        if(!e->active) continue;

        Rectangle source = {
            .x = e->currentframe * frameW,
            .y = 0,
            .width = frameW,
            .height = frameH
        };
        Rectangle dest = {
            .x = e->position.x - drawW/2.0f,   // centered on the explosion point
            .y = e->position.y - drawH/2.0f,
            .width = drawW,
            .height = drawH
        };
        DrawTexturePro(textures->bomb_explosion, source, dest, (Vector2){0,0}, 0.0f, WHITE);
    }
}