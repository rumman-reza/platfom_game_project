#include"ground.h"
#include"player.h"
#include"pattern.h"
#include"types.h"
#include <stdlib.h> 
#include"game.h"
Rectangle getGroundcheckRec(GS* gs){
    Rectangle player = getPlayerRect(gs);
    Rectangle groundcheckrec = (Rectangle){
            .height = 2.0f,
            .width = player.width,
            .x= player.x,
            .y = player.y + player.height
    };
    return groundcheckrec;
}

void groundedCheck(GS* gs){
    gs->player.isgrounded = false;
    for(int i = 0; i < MaxChunkNum; i++){
        
        if(CheckCollisionRecs(getGroundcheckRec(gs), gs->gchunk[i].groundChunkRect)){
            gs->player.isgrounded = true;
            gs->player.position.y = gs->gchunk[i].groundChunkRect.y  - gs->player.collisionOffset.y  - gs->player.height;
            gs->player.velocity.y = 0;
            break;
        }
    }
}
void pushgroundchunk(GS *gs,float x,float y,float height,float width,bool has_health_item)
{
    int index = gs->chunk_index;

    gs->gchunk[index].groundChunkRect =
        (Rectangle){
            .x = x,
            .y = y,
            .width = width,
            .height = height
        };

        
        
    gs->chunk_index = (gs->chunk_index + 1) % MaxChunkNum;
}

void spawn_healthrect(GS* gs,float x,float y,float width){
    int index = gs->chunk_index;
    
    gs->gchunk[index].hasHealthItem = true;

    gs->gchunk[index].healthItemCollected = false;

    gs->gchunk[index].healthItemRect = (Rectangle){
        .x = x + width * 0.5f - 25.0f,
        .y = y - 50.0f,
        .width = 50.0f,
        .height = 50.0f
    };
}


void addspike(GS* gs,float x,float y,float width,float height)
{
    int index = gs->spike_index;

    gs->spikes[index].rect =
        (Rectangle){
            .x = x,
            .y = y,
            .width = width,
            .height = height
        };

    gs->spikes[index].isactive = true;

    gs->spike_index =
        (gs->spike_index + 1) % max_spikes;
}


void updateGround(GS* gs)
{
    float view_distance =
        gs->player.position.x + s_width;

    while(gs->next_spawn_point < view_distance)
    {
        float distSincePattern = gs->next_spawn_point - gs->lastPatternEndX;

        if(distSincePattern >= gs->gapBetweenTheNextPattern)
        {
            const pattern *p = &all_patterns[GetRandomValue(0, pattern_count - 1)];

            spawn_pattern(gs,p,gs->next_spawn_point,ground_y,s_height);

            float patternWidth =getPatternWidth(p) * pattern_tile_width;

            gs->next_spawn_point += patternWidth;

            gs->lastPatternEndX = gs->next_spawn_point;

            gs->gapBetweenTheNextPattern = (float)GetRandomValue(1600, 2500);
        }
        else
        {
            pushgroundchunk(gs,gs->next_spawn_point,ground_y,s_height - ground_y,s_width, true);

            
            if (GetRandomValue(1, 100) <= 10) { 
                
                 float bombX = gs->next_spawn_point + (float)GetRandomValue(0, s_width - bomb_width);
                 float bombY = ground_y - bomb_height;
                 addbomb(gs, bombX, bombY, bomb_width, bomb_height);
            }

            gs->next_spawn_point += s_width;
        }
    }
}

void drawSpikes(GS* gs)
{
    for(int i = 0; i < max_spikes; i++)
    {
        if(!gs->spikes[i].isactive)
            continue;

        Rectangle r = gs->spikes[i].rect;
        
        Rectangle source = {
        .x = 0,
        .y = 0,
        .width  = 16,
        .height = 16
    };
    Rectangle dest = {
        .x = r.x,
        .y = r.y,
        .width  = r.width,
        .height = r.height
    };
    DrawTexturePro(gs->spikes[i].spike_sprite, source, dest, (Vector2){0,0}, 0.0f, WHITE);
        
    }
}



void addbomb(GS* gs, float x, float y, float width, float height) {
    int index = gs->bomb_index;
    gs->bombs[index].rect = (Rectangle){
        .x = x,
        .y = y,
        .width = width,
        .height = height
    };
    gs->bombs[index].isactive = true;
    gs->bomb_index = (gs->bomb_index + 1) % max_bombs;
}

void drawBombs(GS* gs, tex* textures) {
    for (int i = 0; i < max_bombs; i++) {
        if (!gs->bombs[i].isactive) continue;

        Rectangle r = gs->bombs[i].rect;
        
// Image load na holeo jeno lal box dekha jay   (Debuger jonno)
        // DrawRectangleRec(r, RED); 

        // Jodi bomb  sprite load hoye thake, tobe tar upor image ta draw hobe
        if (textures->bomb_sprite.id != 0) {
            Rectangle source = {0, 0, textures->bomb_sprite.width, textures->bomb_sprite.height};
            Rectangle dest = {r.x, r.y, r.width, r.height};
            DrawTexturePro(textures->bomb_sprite, source, dest, (Vector2){0,0}, 0.0f, WHITE);
        }
    }
}