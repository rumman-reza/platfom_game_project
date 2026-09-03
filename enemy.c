#include"enemy.h"
#include"animation.h"
#include<math.h>

Rectangle getEnemyRect(Enemy* enemy){    
    return (Rectangle) {.height = enemy->height, .width = enemy->width, .x=enemy->position.x, .y=enemy->position.y};
}

Enemy loadEnemy(tex* tex){
    Enemy enemy = {0};
  
    enemy.current_enemy_anim_name = enemy_running;
    enemy.state = walking_enemy;

    anim* en_idle = &enemy.enemy_animations[enemy_idle];
    en_idle->tex = tex->enemy_idle;
    en_idle->framecount = 11;
    en_idle->frameduration = .08f;
    en_idle->frameHeight = tex->enemy_idle.height;
    en_idle->frameWidth = tex->enemy_idle.width/en_idle->framecount;
    en_idle->looping = true;
    en_idle->timedependent = true;

    anim* en_attack = &enemy.enemy_animations[enemy_attack];
    en_attack->tex = tex->enemy_attack;
    en_attack->framecount = 18;
    en_attack->frameduration = .08f;
    en_attack->frameHeight = tex->enemy_attack.height;
    en_attack->frameWidth = tex->enemy_attack.width/en_attack->framecount;
    en_attack->looping = true;
    en_attack->timedependent = true;

    anim* en_run = &enemy.enemy_animations[enemy_running];
    en_run->tex = tex->enemy_run;
    en_run->framecount = 13;
    en_run->frameduration = .08f;
    en_run->frameHeight = tex->enemy_run.height;
    en_run->frameWidth = tex->enemy_run.width/en_run->framecount;
    en_run->looping = true;
    en_run->timedependent = true;

    enemy.facing_left = true; // change this
    enemy.height = tex->idle.height*SPRITE_SCALE*1.8f;
    enemy.width = tex->idle.height*SPRITE_SCALE*1.8f;
    enemy.isactive = true;
    enemy.isgrounded = true;
    enemy.inital_position = (Vector2){s_width,s_height*3.7f/4-enemy.height};
    enemy.position = enemy.inital_position;

    return enemy;

}
void UnloadEnemyAnims(Enemy *e) {
    for (int i = 0; i < enemy_anim_num; i++) {
        if (e->enemy_animations[i].tex.id != 0) UnloadTexture(e->enemy_animations[i].tex);
    }
}
void drawEnemy(Enemy* enemy){
    if(!enemy->isactive) return;

    anim* frame = &enemy->enemy_animations[enemy->current_enemy_anim_name];

    float drawWidth  = frame->frameWidth  * SPRITE_SCALE * 1.8f;
    float drawHeight = frame->frameHeight * SPRITE_SCALE * 1.8f;
    float groundY = s_height*3.7f/4;
    Rectangle source = {
        .x = frame->currentframe*frame->frameWidth,
        .y = 0,
        .width = (enemy->facing_left)? -frame->frameWidth:frame->frameWidth,
        .height = frame->frameHeight
    };
    Rectangle dest = {
        .x = enemy->position.x,
        .y = groundY-drawHeight,
        .width = drawWidth,
        .height = drawHeight 
    };
    DrawTexturePro(frame->tex,source,dest,(Vector2){0.0f,0.0f},0.0f,WHITE);

}

void updateEnemyAnimation(Enemy* enemy,enemy_anim en_anim){
    if(enemy->current_enemy_anim_name != en_anim){
        enemy->current_enemy_anim_name = en_anim;
        enemy->enemy_animations[en_anim].currentframe = 0;
        enemy->enemy_animations[en_anim].frametimer=0;

    }
}

void updateEnemy(Enemy* enemy,GS* gs,float dt){
    if(!enemy->isactive) return;
    Player* player = &gs->player;
    
    float player_center_x = player->position.x + player->width/2.0f;
    float enemey_center_x = enemy->position.x + enemy->width/2.0f;
    float dist = fabsf(player_center_x-enemey_center_x);
    
    enemy->facing_left = (player_center_x<enemey_center_x);
    
    if(enemy->attack_cooldown>=0) enemy->attack_cooldown-=dt;
    
    switch (enemy->state)
    {
        case walking_enemy:{
            if(dist<=attackrange){
                enemy->state = attacking_enemy;
                enemy->velocity.x = 0;  
                break;
            }
            float dir = (enemy->facing_left)? -1.0f:1.0f;
            enemy->velocity.x = dir*enSpeed;
            enemy->position.x += enemy->velocity.x*dt;
            updateEnemyAnimation(enemy,enemy_running);
            break;
        }
        case attacking_enemy: {
            enemy->velocity.x = 0;
            if(dist>attackrange) {
                enemy->state = walking_enemy;
                break;
            }
            if(enemy->attack_cooldown<=0){
                // enemy->position.x += 19;
                updateEnemyAnimation(enemy,enemy_attack);
                enemy->attack_cooldown = encooldown;
                
                
            }
            // else if(enemy->current_enemy_anim_name == enemy_attack && enemy->enemy_animations[enemy_attack].currentframe>= enemy->enemy_animations[enemy_attack].framecount - 1){
                
            //     updateEnemyAnimation(enemy, enemy_idle);
            //     enemy->enemy_animations[enemy_idle].currentframe = 0;
            // }
            break;
        }
        
        default:
        break;
    }
    updateAnimation(&enemy->enemy_animations[enemy->current_enemy_anim_name],dt); 

}