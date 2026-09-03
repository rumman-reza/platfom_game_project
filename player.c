#include"player.h"
#include "raymath.h"
void setAnimation(GS* gs,anim_name name){
    if(gs->current_player_anim_name != name){
        gs->current_player_anim_name = name;
        gs->player_animations[name].currentframe=0;
        gs->player_animations[name].frametimer=0;
    }
}

Rectangle getPlayerRect(GS* gs){
    return (Rectangle){gs->player.position.x,gs->player.position.y,gs->player.width,gs->player.height};
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
    Player* a = &gs->player;

    if(a->dashcooldowntimer>=0) a->dashcooldowntimer-=dt;

    if(IsKeyPressed(KEY_LEFT_SHIFT) && !a->isdashing && a->dashcooldowntimer<=0 && a->isgrounded){
        a->isdashing = true;
        a->dashcooldowntimer =dash_cooldowntimer;
        a->dashduration = dash_duration;
        a->velocity.x = (a->facing_left)? -dash_speed : dash_speed;
        a->velocity.y = -260.0f;
        setAnimation(gs,player_dash);
    }
    if(a->dashduration>=0){
        a->dashduration-=dt;
        
        if(a->dashduration<=0){
            a->isdashing = false;
            a->velocity.x = 0;
        }
    }
}
Rectangle gethitbox(GS* gs){
    Player* p = &gs->player;
    float hitbox_height = p->height;
    float hitbox_width = 40.0f*SPRITE_SCALE;
    Rectangle hitbox = (Rectangle){
        .height = hitbox_height,
        .width = hitbox_width,
        .x = (p->facing_left)? p->position.x-hitbox_width : p->position.x+hitbox_width,
        .y = p->position.y
    };
    return hitbox;
}

void hitting(GS* gs,float dt){
    Player* p = &gs->player;
    anim* a = &gs->player_animations[gs->current_player_anim_name];
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !p->isattacking && !p->isdashing && p->isgrounded){
        p->isattacking = true;
        p->hitduration = attackduration;
        setAnimation(gs,attack);
    }else if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !p->isattacking && !p->isdashing && !p->isgrounded){
        p->isattacking = true;
        p->hitduration = airattackduration;
        setAnimation(gs,airattack);
    }
    if(p->isattacking){
        p->hitduration-=dt;
        if(attackstartframe<= a->currentframe && a->currentframe <= attackendframe){
            Rectangle hitbox = gethitbox(gs);
            // check collision of hitbox with enemy rectangle then decrease enemy health
        }
        if(p->hitduration<=0){
            p->isattacking = false;
            setAnimation(gs,player_idle);
        }
    }
}

void updateJumpFrame(GS* gs){
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

void updateHealth(GS* gs, float dt){
    if(gs->player.isDead) return;
    gs->player.health -= HEALTH_DECAY_RATE * dt;
    if(gs->player.health <= 0.0f){
        gs->player.health = 0.0f;
        gs->player.isDead = true;
        setAnimation(gs,player_die);
    }
}

void Gravity(GS* gs,float dt){
    gs->player.velocity.y += gravity*dt;
}
void playerMovement(GS* gs,anim* anim,float dt){


    if(gs->player.isdashing){ 
        gs->player.position = (Vector2)Vector2Add(gs->player.position,Vector2Scale(gs->player.velocity,dt));
        return;
    }
    //check button input
    if(IsKeyDown(KEY_D) && gs->player.isgrounded){ 

        if(gs->player.isattacking) gs->player.velocity.x = pAttackMoveSpeed;
        else gs->player.velocity.x = pSpeed;

        gs->player.facing_left=false;

        if(!gs->player.isattacking) setAnimation(gs,player_running);
    }
    else if(IsKeyDown(KEY_D) && !gs->player.isgrounded){
        gs->player.velocity.x = pSpeedAir;
        gs->player.facing_left=false;
    }
    
    else if(IsKeyDown(KEY_A)&& gs->player.isgrounded ) {
        if(gs->player.isattacking) gs->player.velocity.x = -pAttackMoveSpeed;
        else gs->player.velocity.x = -pSpeed;
        gs->player.facing_left=true;
        if(!gs->player.isattacking) setAnimation(gs,player_running);
    }
    else if(IsKeyDown(KEY_A) && !gs->player.isgrounded){
        gs->player.velocity.x = -pSpeedAir;
        gs->player.facing_left=true;
    }

    else{ 
        gs->player.velocity.x=0;
        if(gs->player.isgrounded && !gs->player.isattacking) setAnimation(gs,player_idle);
        else if(!gs->player.isgrounded && !gs->player.isattacking)setAnimation(gs,player_jump);
    }


    if(IsKeyPressed(KEY_SPACE) && gs->player.isgrounded){
        gs->player.velocity.y = -jumpSpeed;
        gs->player.isgrounded = false; 
        setAnimation(gs,player_jump);
    }

    //update the player postion after taking input
    gs->player.position = (Vector2) Vector2Add(gs->player.position,Vector2Scale(gs->player.velocity,dt));

}
