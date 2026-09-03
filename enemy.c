#include"enemy.h"
#include"animation.h"
Rectangle getEnemyRect(Enemy* enemy){    
    return (Rectangle) {.height = enemy->height, .width = enemy->width, .x=enemy->position.x, .y=enemy->position.y};
}

Enemy loadEnemy(tex* tex){
    Enemy enemy = {0};
    enemy.facing_left = true; // change this
    enemy.height = tex->enemy_idle.height*SPRITE_SCALE*1.8f;
    enemy.width = tex->enemy_idle.width*SPRITE_SCALE*1.8f;
    enemy.isactive = true;
    enemy.isgrounded = true;
    enemy.inital_position = (Vector2){s_width,s_height*3.7f/4-enemy.height};
    enemy.position = enemy.inital_position;


    enemy.current_enemy_anim_name = enemy_idle;

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

    Rectangle source = {
        .x = frame->currentframe*frame->frameWidth,
        .y = 0,
        .width = (enemy->facing_left)? -frame->frameWidth:frame->frameWidth,
        .height = frame->frameHeight
    };
    Rectangle dest = {
        .x = enemy->position.x,
        .y = enemy->position.y,
        .width = frame->frameWidth*SPRITE_SCALE*1.8f,
        .height = frame->frameHeight*SPRITE_SCALE*1.8f
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

void updateEnemy(Enemy* enemy,float dt){
    if(!enemy->isactive) return;
    updateAnimation(&enemy->enemy_animations[enemy->current_enemy_anim_name],dt); 
    
    


}