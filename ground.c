#include"ground.h"
#include"player.h"
#include"pattern.h"
#include"types.h"
#include <stdlib.h> 
#include"game.h"
Rectangle getGroundcheckRec(GS* gs){
    Rectangle player = getPlayerRect(gs);
    Rectangle groundcheckrec = (Rectangle){
            .height = 1.0f,
            .width = player.width,
            .x= player.x + player.width/2.0f,
            .y = player.y + player.height
    };
    return groundcheckrec;
}

void groundedCheck(GS* gs, float dt){
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

    gs->gchunk[index].hasHealthItem = has_health_item;
    gs->gchunk[index].healthItemCollected = false;

    if (has_health_item)
    {
        gs->gchunk[index].healthItemRect =
            (Rectangle){
                .x = x + width * 0.5f - 15.0f,
                .y = y - 30.0f,
                .width = 30.0f,
                .height = 30.0f};
    }

    gs->chunk_index =
        (gs->chunk_index + 1) % MaxChunkNum;
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

        Vector2 top = {
            r.x + r.width * 0.5f,
            r.y
        };

        Vector2 bottomL = {
            r.x,
            r.y + r.height
        };

        Vector2 bottomR = {
            r.x + r.width,
            r.y + r.height
        };

        DrawTriangle(
            bottomL,
            bottomR,
            top,
            RED
        );
    }
}
