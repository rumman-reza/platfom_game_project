#include "game.h"
#include "texture.h"
#include "animation.h"
#include "player.h"
#include "enemy.h"
#include "ground.h"
#include "background.h"
#include "camera.h"
#include "health.h"

void drawGame(GS* gs){

//drawing background elements

    drawBackground(gs);

//drawing the ground rectangles;
    for(int i=0;i<MaxChunkNum;i++){
        DrawRectangleRec(gs->gchunk[i].groundChunkRect,DARKBROWN);
        // DrawRectangleLinesEx(gs->gchunk[i].groundChunkRect,3,BLACK);


//    ei chunk e heal item thakle  and seta pick na kore
//thakle box ta green 
if(gs->gchunk[i].hasHealthItem && !gs->gchunk[i].healthItemCollected){
            DrawRectangleRec(gs->gchunk[i].healthItemRect, GREEN);
        }
    }    
//drawing player sprite
    drawPlayerSprite(gs);
}

void initGame(GS* gs,tex* tex,anim* anim){

    float ground_y = s_height*3.7f/4;
    float ground_height = s_height-ground_y;
    SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);

// load textures
    loadTexture(tex,gs);
// load animations
    loadAnimation(gs,tex,anim);

//set player
    gs->player.height = gs->player_animations[player_idle].frameHeight * SPRITE_SCALE*1.6f;
    gs->player.width  = gs->player_animations[player_idle].frameWidth  * SPRITE_SCALE*1.6f;

    gs->player.initial_position.x=s_width/2.0f;
    gs->player.initial_position.y=ground_y-gs->player.height;

    gs->player.position.x=s_width/2.0f;
    gs->player.position.y=ground_y-gs->player.height;
    

//set camera 
    gs->camera.offset = (Vector2){s_width/2.0f,0.0f};
    gs->camera.rotation = 0.0f;
    gs->camera.zoom = 1.0f;

    float player_center_x = gs->player.position.x + gs->player.width/2.0f;
    gs->camera.target = (Vector2){player_center_x-camera_half_deadzone,0.0f};
    gs->last_camera_x = gs->camera.target.x;
// setup initial Ground
    gs->next_spawn_point=-s_width;
    gs->chunk_index=0;

//heath function er variable gulo
    gs->player.maxHealth = PLAYER_MAX_HEALTH;
    gs->player.health = PLAYER_MAX_HEALTH;
    gs->player.isDead = false;



for(int i=0;i<MaxChunkNum;i++){
        gs->gchunk[i].groundChunkRect = (Rectangle){gs->next_spawn_point,ground_y,s_width,ground_height};
        gs->gchunk[i].hasHealthItem = true; 
        gs->gchunk[i].healthItemCollected = false;
        gs->gchunk[i].healthItemRect = (Rectangle){
            .x = gs->next_spawn_point + (s_width * 0.8f),
            .y = ground_y - 40.0f,
            .width = 30.0f,
            .height = 30.0f
        };
        
        
        gs->next_spawn_point+=s_width;
    }
    // setup background layers — farthest (slowest apparent motion) to nearest
    float bg_scrollfactors[BG_LAYER_COUNT] = {0.1f, 0.25f, 0.45f,0.65f , 0.85f,.95f};
    
    for(int i=0;i<BG_LAYER_COUNT;i++){
        parallax_layer *l = &gs->bgLayers[i];
        l->scrollfactor = bg_scrollfactors[i];
        l->offsetX = 0.0f;
        l->source = (Rectangle){
            .x = 0, .y = 0,
            .width  = l->tex.width,
            .height = l->tex.height
        };
    }
}

void updateGame(GS* gs,anim* anim,float dt){
    switch(gs->currentscreen){
        case GAME: updateGameplay(gs,anim,dt); break;
    }
}


void updateGameplay(GS* gs,anim* anim,float dt){
    updateAnimation(&gs->player_animations[gs->current_player_anim_name],dt);
    playerDashUpdate(gs,dt);
    Gravity(gs,dt);
    updateHealth(gs,dt);
    hitting(gs,dt);
    playerMovement(gs,anim,dt);

    checkHealthPickup(gs); // collision ditect check 

    updateJumpFrame(gs);
    updateGround(gs);
    groundedCheck(gs,dt);
    cameraMovement(gs);
    restrict_left_movement(gs);

    float cameradelta = gs->camera.target.x - gs->last_camera_x;
    updateParallax(gs,cameradelta);
    gs->last_camera_x = gs->camera.target.x;
}


