#include"player.h"
#include "raymath.h"
#include"types.h"
#include"enemy.h"
#include<stdio.h>

void setplayerstate(GS* gs){
    playerstate newstate;
    Player* p = &gs->player;
    anim_name newanim;
    if(p->isDead){
        newanim = player_die;
        newstate = dead_player;
    }else if(p->invultimer>0.0f && gs->current_player_anim_name==player_hurt && !gs->player_animations[gs->current_player_anim_name].isfinished){
        // jokhon kono animation emne cholte thakbe shesh na howa porjonto tokhon eivabe korte hobe
        newanim = player_hurt;
        newstate = hurting_player;
    }else if(p->isdashing){
        newanim = player_dash;
        newstate = dashing_player;
    }else if(p->isattacking && !p->isgrounded){
        newanim = airattack;
        newstate = attacking_player;
    }else if(p->isattacking){
        newanim = attack;
        newstate = attacking_player;
    }
    else if(!p->isgrounded){
        newanim = player_jump;
        newstate = jumping_player;
    }
    else if (fabs(p->velocity.x)>10.0f){
        newanim = player_running;
        newstate = running_player;

    }else {
        newanim = player_idle;
        newstate = idle_player;
    }
    if(newanim!=gs->current_player_anim_name){
        gs->current_player_anim_name = newanim;
        gs->player_animations[newanim].currentframe = 0;
        gs->player_animations[newanim].frametimer = 0;
        gs->player_animations[newanim].isfinished =false;
    }
    if(gs->current_player_state!=newstate) gs->current_player_state = newstate;
}

Rectangle getPlayerRect(GS* gs){
    return (Rectangle){
        gs->player.position.x + gs->player.collisionOffset.x,
        gs->player.position.y + gs->player.collisionOffset.y,
        gs->player.width,
        gs->player.height
    };
}
float getPlayerCenterX(GS* gs){
    return gs->player.position.x + gs->player.collisionOffset.x + gs->player.width/2.0f;
}

void drawPlayerSprite(GS* gs){
    anim *a = &gs->player_animations[gs->current_player_anim_name];

    Rectangle source = {
        .x = a->currentframe * a->frameWidth,
        .y = 0,
        .width  = (gs->player.facing_left) ? -a->frameWidth : a->frameWidth,
        .height = a->frameHeight
    };
    Rectangle dest = {
        .x = gs->player.position.x,
        .y = gs->player.position.y,
        .width  = a->frameWidth  * SPRITE_SCALE*1.6f,
        .height = a->frameHeight * SPRITE_SCALE*1.6f
    };
    DrawTexturePro(a->tex, source, dest, (Vector2){0,0}, 0.0f, WHITE);
}

void playerDashUpdate(GS* gs,float dt){
    if(gs->current_player_state>dashing_player) return;
    Player* a = &gs->player;

    if(a->dashcooldowntimer>=0) a->dashcooldowntimer-=dt;

    if(IsKeyPressed(KEY_LEFT_SHIFT) && !a->isdashing && a->dashcooldowntimer<=0 && a->isgrounded){
        a->isdashing = true;
        a->dashcooldowntimer =dash_cooldowntimer;
        a->dashduration = dash_duration;
        a->velocity.x = (a->facing_left)? -dash_speed : dash_speed;
        a->velocity.y = -260.0f;
    }
    if(a->dashduration>=0){
        a->dashduration-=dt;
        
        if(a->dashduration<=0){
            a->isdashing = false;
            a->velocity.x = 0;
        }
    }
}
Rectangle getplayerhitbox(GS* gs){
    Player* p = &gs->player;
    Rectangle body = getPlayerRect(gs);
    float reach = 60.0f;
    float x = p->facing_left ?body.x - reach: body.x + body.width;
    return (Rectangle){ x, body.y, reach, body.height };
}

void hitting(GS* gs,float dt){
    if(gs->current_player_state>attacking_player) return;

    Player* p = &gs->player;
    anim* a = &gs->player_animations[gs->current_player_anim_name];
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !p->isattacking && !p->isdashing && p->isgrounded){
        p->isattacking = true;
        p->hitduration = attackduration;
    }else if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !p->isattacking && !p->isdashing && !p->isgrounded){
        p->isattacking = true;
        p->hitduration = airattackduration;

    }
}

void updateJumpFrame(GS* gs){
    if(gs->current_player_state>jumping_player) return;
    if(gs->player.isgrounded && gs->current_player_anim_name != player_jump) return;
    else {
        if(gs->player.velocity.y<0) gs->player_animations[player_jump].currentframe=0;
        else if(gs->player.velocity.y>250.0f) gs->player_animations[player_jump].currentframe=1;
    }
}

void restrict_left_movement(GS* gs){
    if(gs->player.position.x<gs->camera.target.x-s_width/2.0f){
        gs->player.position.x = gs->camera.target.x-s_width/2.0f;
    }
}


void Gravity(GS* gs,float dt){
    gs->player.velocity.y += gravity*dt;
}
void playerMovement(GS* gs,anim* anim,float dt){
    if(gs->player.isDead || gs->current_player_state==hurting_player){
        gs->player.position.y = gs->player.position.y + gs->player.velocity.y*dt;
        return;
    }

    if(gs->player.isdashing){ 
        gs->player.position = (Vector2)Vector2Add(gs->player.position,Vector2Scale(gs->player.velocity,dt));
        return;
    }

    //check button input
    if(IsKeyDown(KEY_D) && gs->player.isgrounded){ 

        if(gs->player.isattacking) gs->player.velocity.x = pAttackMoveSpeed;
        else gs->player.velocity.x = pSpeed;

        gs->player.facing_left=false;

    }
    else if(IsKeyDown(KEY_D) && !gs->player.isgrounded){
        gs->player.velocity.x = pSpeedAir;
        gs->player.facing_left=false;
    }
    
    else if(IsKeyDown(KEY_A)&& gs->player.isgrounded ) {
        if(gs->player.isattacking) gs->player.velocity.x = -pAttackMoveSpeed;
        else gs->player.velocity.x = -pSpeed;
        gs->player.facing_left=true;
    }
    else if(IsKeyDown(KEY_A) && !gs->player.isgrounded){
        gs->player.velocity.x = -pSpeedAir;
        gs->player.facing_left=true;
    }

    else{ 
        gs->player.velocity.x=0;
        // if(gs->player.isgrounded && !gs->player.isattacking) setAnimation(gs,player_idle);
        // else if(!gs->player.isgrounded && !gs->player.isattacking)setAnimation(gs,player_jump);
    }


    if(IsKeyPressed(KEY_SPACE) && gs->player.isgrounded){
        gs->player.velocity.y = -jumpSpeed;
        gs->player.isgrounded = false; 
    }

    //update the player postion after taking input
    gs->player.position = (Vector2) Vector2Add(gs->player.position,Vector2Scale(gs->player.velocity,dt));

}

void checkHealthPickup(GS* gs){
    if(gs->player.isDead) return;
    Rectangle playerRect = getPlayerRect(gs);

    for(int i = 0; i < MaxChunkNum; i++){
        // item jodi thake and seta pick na kora hoye thake tobe 
        if(gs->gchunk[i].hasHealthItem && !gs->gchunk[i].healthItemCollected){
            if(CheckCollisionRecs(playerRect, gs->gchunk[i].healthItemRect)){
                gs->player.health += 25.0f; // health joyp kore barate chai 
                
                // max health theke besi houya jabe na tai 
                //max er theke besi hote laglei max ei rakhbo 
                if(gs->player.health > gs->player.maxHealth){
                    gs->player.health = gs->player.maxHealth;
                }

                gs->gchunk[i].healthItemCollected = true; // ekbar pick korle oita r dekha jabe na 
            }
        }
    }
}