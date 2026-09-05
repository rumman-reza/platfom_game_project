#include"ground.h"
#include"player.h"

#include <stdlib.h> 

void groundedCheck(GS* gs, float dt){
    gs->player.isgrounded = false;
    for(int i = 0; i < MaxChunkNum; i++){
        if(CheckCollisionRecs(getPlayerRect(gs), gs->gchunk[i].groundChunkRect)){
            gs->player.isgrounded = true;
            gs->player.position.y = gs->gchunk[i].groundChunkRect.y 
                                     - gs->player.collisionOffset.y 
                                     - gs->player.height;
            gs->player.velocity.y = 0;
            break;
        }
    }
}

void updateGround(GS* gs){
    float view_distance = gs->player.position.x + s_width;
    while(gs->next_spawn_point<view_distance){
        gs->gchunk[gs->chunk_index].groundChunkRect.x = gs->next_spawn_point;
        
    
        gs->gchunk[gs->chunk_index].hasHealthItem =(true);  

         gs->gchunk[gs->chunk_index].healthItemCollected = false; 
            //notun chunk hole item uncollected thakbe 
        
            if(gs->gchunk[gs->chunk_index].hasHealthItem){
            gs->gchunk[gs->chunk_index].healthItemRect = (Rectangle){
               .x = gs->next_spawn_point + (s_width * 0.5f), 
               //chunk er thik majh borabor 
                .y = gs->gchunk[gs->chunk_index].groundChunkRect.y - 30.0f, // ground er thik upore 
                .width = 30.0f,
                .height = 30.0f
            };
           
        }
        
        gs->next_spawn_point += s_width;
        gs->chunk_index = (gs->chunk_index+1) % MaxChunkNum;
    }
}