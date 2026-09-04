#include"animation.h"

void loadAnimation(GS* gs,tex* tex,anim* animt){

    gs->player_animations[player_idle].tex = tex->idle;
    gs->player_animations[player_idle].framecount = 1;
    gs->player_animations[player_idle].frameduration = 0.1f;
    gs->player_animations[player_idle].frameHeight = gs->player_animations[player_idle].tex.height;
    gs->player_animations[player_idle].frameWidth = gs->player_animations[player_idle].tex.width/gs->player_animations[player_idle].framecount;
    gs->player_animations[player_idle].timedependent = true;
    gs->player_animations[player_idle].looping = true;
    gs->player_animations[player_idle].isfinished = false;   
    gs->player_animations[player_running].tex = tex->running;
    gs->player_animations[player_running].framecount = 8;
    gs->player_animations[player_running].frameduration = 0.08f;
    gs->player_animations[player_running].frameHeight = gs->player_animations[player_running].tex.height;
    gs->player_animations[player_running].frameWidth = gs->player_animations[player_running].tex.width/gs->player_animations[player_running].framecount;
    gs->player_animations[player_running].timedependent = true;
    gs->player_animations[player_running].looping = true;
    gs->player_animations[player_running].isfinished = false;
    

    gs->player_animations[player_jump].tex = tex->jumpandfall;
    gs->player_animations[player_jump].framecount=2;
    gs->player_animations[player_jump].timedependent=false;
    gs->player_animations[player_jump].frameHeight = gs->player_animations[player_jump].tex.height;
    gs->player_animations[player_jump].frameWidth = gs->player_animations[player_jump].tex.width/gs->player_animations[player_jump].framecount;
    gs->player_animations[player_jump].looping = false;
    gs->player_animations[player_jump].isfinished = false;

    gs->player_animations[player_dash].tex = tex->dash;
    gs->player_animations[player_dash].framecount = 4;
    gs->player_animations[player_dash].frameduration = 0.08f;
    gs->player_animations[player_dash].timedependent=true;
    gs->player_animations[player_dash].frameHeight=  gs->player_animations[player_dash].tex.height;
    gs->player_animations[player_dash].frameWidth=  gs->player_animations[player_dash].tex.width/6;
    gs->player_animations[player_dash].looping = false;
    gs->player_animations[player_dash].isfinished = false;

    anim* attack_ani = &gs->player_animations[attack];
    attack_ani->tex = tex->attack1;
    attack_ani->framecount = 9;
    attack_ani->frameduration = .05f;
    attack_ani->frameHeight = attack_ani->tex.height;
    attack_ani->frameWidth = attack_ani->tex.width/14;
    attack_ani->looping = true;
    attack_ani->timedependent = true;
    attack_ani->isfinished = false;

    anim* air_attack_ani = &gs->player_animations[airattack];
    air_attack_ani->tex = tex->air_attack1;
    air_attack_ani->framecount = 7;
    air_attack_ani->frameduration = .08f;
    air_attack_ani->frameHeight = air_attack_ani->tex.height;
    air_attack_ani->frameWidth = air_attack_ani->tex.width/air_attack_ani->framecount;
    air_attack_ani->looping = true;
    air_attack_ani->timedependent = true;
    air_attack_ani->isfinished = false;

    anim* p_die = &gs->player_animations[player_die];
    p_die->tex = tex->die;
    p_die->framecount = 4;
    p_die->frameduration = 0.08f;
    p_die->frameHeight = tex->die.height;
    p_die->frameWidth = tex->die.width/p_die->framecount;
    p_die->timedependent = true;
    p_die->looping = false;
    p_die->isfinished = false;

    anim* p_hurt = &gs->player_animations[player_hurt];
    p_hurt->tex = tex->hurt;
    p_hurt->framecount = 1;
    p_hurt->frameduration = 0.08f;
    p_hurt->frameHeight = tex->hurt.height;
    p_hurt->frameWidth = tex->hurt.width/p_die->framecount;
    p_hurt->timedependent = true;
    p_hurt->looping = false;
    p_hurt->isfinished = false;


    gs->current_player_anim_name = player_idle;

}

void updateAnimation(anim* a,float dt){
    
    if(!a->timedependent) return;

    a->frametimer += dt;

    while(a->frametimer>=a->frameduration){
        a->frametimer-=a->frameduration;
        if(a->looping){
            a->currentframe = (a->currentframe + 1)%(a->framecount);
        }
        else if(a->currentframe < a->framecount-1 ) a->currentframe++;

        if(!a->looping && a->currentframe >= a->framecount-1) a->isfinished = true;
    }

}
