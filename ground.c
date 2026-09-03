#include"ground.h"
#include"player.h"

void groundedCheck(GS* gs,float dt){
    
    for(int i=0;i<MaxChunkNum;i++){
        if(CheckCollisionRecs(getPlayerRect(gs),gs->gchunk[i].groundChunkRect)){
            gs->player.isgrounded=true;
            gs->player.position.y = gs->gchunk[0].groundChunkRect.y-gs->player.height;
            gs->player.velocity.y=0;
            break;
        }
    else gs->player.isgrounded=false;

    }
}
void updateGround(GS* gs){
    float view_distance = gs->player.position.x + s_width;
    while(gs->next_spawn_point<view_distance){
        gs->gchunk[gs->chunk_index].groundChunkRect.x = gs->next_spawn_point;
        gs->next_spawn_point += s_width;
        gs->chunk_index = (gs->chunk_index+1) % MaxChunkNum;
    }
}