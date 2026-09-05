    #include"enemy.h"
    #include"animation.h"
    #include<math.h>
    #include"player.h"

    float timer=0.0f;

    Rectangle getEnemyRect(Enemy* enemy){    
        return (Rectangle) {.height = enemy->height, .width = enemy->width, .x=enemy->position.x, .y=enemy->position.y};
    }

    Rectangle getEnemyHitbox(Enemy* enemy){
        Rectangle body = getEnemyRect(enemy);
        float reach = 50.0f;
        float x = enemy->facing_left ? body.x - reach: body.x + body.width ;
        return (Rectangle){ x, body.y, reach, body.height };
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
        en_idle->isfinished = false;

        anim* en_attack = &enemy.enemy_animations[enemy_attack];
        en_attack->tex = tex->enemy_attack;
        en_attack->framecount = 18;
        en_attack->frameduration = .05f;
        en_attack->frameHeight = tex->enemy_attack.height;
        en_attack->frameWidth = tex->enemy_attack.width/en_attack->framecount;
        en_attack->looping = false;
        en_attack->timedependent = true;
        en_attack->isfinished = false;

        anim* en_run = &enemy.enemy_animations[enemy_running];
        en_run->tex = tex->enemy_run;
        en_run->framecount = 13;
        en_run->frameduration = .08f;
        en_run->frameHeight = tex->enemy_run.height;
        en_run->frameWidth = tex->enemy_run.width/en_run->framecount;
        en_run->looping = true;
        en_run->timedependent = true;
        en_run->isfinished = false;

        anim* en_dead = &enemy.enemy_animations[enemy_dead];
        en_dead->tex = tex->enemy_dead;
        en_dead->framecount = 15;
        en_dead->frameduration = .06f;
        en_dead->frameHeight = tex->enemy_dead.height;
        en_dead->frameWidth = tex->enemy_dead.width/en_dead->framecount;
        en_dead->looping = false;
        en_dead->timedependent = true;
        en_dead->isfinished = false;  

        anim* en_hurt = &enemy.enemy_animations[enemy_hurt];
        en_hurt->tex = tex->enemy_hurt;
        en_hurt->framecount = 8;
        en_hurt->frameduration = .08f;
        en_hurt->frameHeight = tex->enemy_hurt.height;
        en_hurt->frameWidth = tex->enemy_hurt.width/en_hurt->framecount;
        en_hurt->looping = false;
        en_hurt->timedependent = true;
        en_hurt->isfinished = false;  
        

        enemy.facing_left = true; // change this
        enemy.height = en_idle->frameHeight*SPRITE_SCALE*1.8f;
        enemy.width = en_idle->frameWidth*SPRITE_SCALE*1.8f;
        enemy.isactive = true;
        enemy.isgrounded = true;
        enemy.inital_position = (Vector2){s_width,s_height*3.7f/4-enemy.height};
        enemy.position = enemy.inital_position;
        enemy.health = enemy_max_health;
        enemy.invultimer = 0.0f;
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
            enemy->enemy_animations[en_anim].isfinished = false;
        }
    }

    void updateEnemy(GS* gs,float dt){

        for(int i=0;i<max_enemy_num;i++){
            Enemy* enemy = &gs->enemy[i];
            
            if(!enemy->isactive) continue;;

            Player* player = &gs->player;

            float player_center_x = getPlayerCenterX(gs);
            float enemey_center_x = enemy->position.x + enemy->width/2.0f;
            float dist = fabsf(player_center_x-enemey_center_x);
            enemy->facing_left = (player_center_x<enemey_center_x);

            if(enemy->attack_cooldown>=0) enemy->attack_cooldown-=dt;
            anim* a = &enemy->enemy_animations[enemy->current_enemy_anim_name];

            switch (enemy->state)
            {
                case dead_enemy:{
                    enemy->velocity.x=0;
                    if(enemy->enemy_animations[enemy->current_enemy_anim_name].isfinished){
                        enemy->isactive = false;
                        enemy->isdead = true;
                    }
                    break;
                }
                case hurting_enemy:{
                    if(enemy->current_enemy_anim_name==enemy_hurt && !enemy->enemy_animations[enemy->current_enemy_anim_name].isfinished){
                        updateEnemyAnimation(enemy,enemy_hurt);
                    }
                    else{ 
                        if(dist<=attackrange){
                            enemy->state = attacking_enemy;
                            enemy->velocity.x = 0;  
                            enemy->hashitplayerthisswing = false; 
                            updateEnemyAnimation(enemy,enemy_attack);
                            enemy->enemy_animations[enemy->current_enemy_anim_name].currentframe = 0;
                            enemy->enemy_animations[enemy->current_enemy_anim_name].isfinished = 0;
                            break;
                        }
                        else{
                            enemy->state = walking_enemy;
                            updateEnemyAnimation(enemy,enemy_running);
                            break;
                        }
                    }
                    break;
                }

                case walking_enemy:{
                    if(dist<=attackrange){
                        enemy->state = attacking_enemy;
                        enemy->current_enemy_anim_name = enemy_attack;
                        enemy->velocity.x = 0;  
                        enemy->hashitplayerthisswing = false; 
                        updateEnemyAnimation(enemy,enemy_attack);
                        //age thekei chilo tai nije theke zero korte hobe
                        enemy->enemy_animations[enemy->current_enemy_anim_name].currentframe = 0;
                        enemy->enemy_animations[enemy->current_enemy_anim_name].isfinished = 0;
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
                    if(dist>attackrange && enemy->current_enemy_anim_name==enemy_attack && a->isfinished) {
                        enemy->state = walking_enemy;
                        updateEnemyAnimation(enemy,enemy_running);

                        break;
                    }
                    if(enemy->attack_cooldown<=0){
                        if (dist <= attackrange){ 
                            enemy->hashitplayerthisswing = false; 
                            updateEnemyAnimation(enemy, enemy_attack);
                            enemy->enemy_animations[enemy->current_enemy_anim_name].currentframe = 0;
                            enemy->enemy_animations[enemy->current_enemy_anim_name].isfinished = 0;
                            enemy->attack_cooldown = encooldown;
                        } else {
                            enemy->state = walking_enemy;
                            updateEnemyAnimation(enemy, enemy_running);
                        }
                    }
                    // else if(enemy->current_enemy_anim_name == enemy_attack && enemy->enemy_animations[enemy_attack].currentframe>= enemy->enemy_animations[enemy_attack].framecount - 1){
                        
                    //     updateEnemyAnimation(enemy, enemy_idle);
                    //     enemy->enemy_animations[enemy_idle].currentframe = 0;
                    // }
                    break;
                }
                
                default:{
                    updateEnemyAnimation(enemy,enemy_idle);
                }
                break;
            }
        }
    }
    void updateEnemyInvultimer(GS* gs,float dt){
        for(int i=0;i<max_enemy_num;i++){
            Enemy* e = &gs->enemy[i];
            if(e->invultimer>=0.0f){
                e->invultimer-=dt;
                if(e->invultimer<=0.0f) e->invultimer = 0.0f;
            }
        }
    }

    void damageEnemy(Enemy* e,float amount){
        if (e->isdead || e->invultimer>0.0f) return;
        e->health -= amount;
        e->invultimer = enemy_invultimer;

        if (e->health <= 0.0f) {
            e->health = 0.0f;
            e->isdead = true;
            e->state = dead_enemy;
            // e->currentFrame = 0;
            updateEnemyAnimation(e,enemy_dead);
        } else {
            e->state = hurting_enemy;
            // e->currentFrame = 0;
            updateEnemyAnimation(e,enemy_hurt);
        }
    }
    void updateEnemyAnimations(GS* gs,float dt){
        for(int i=0;i<max_enemy_num;i++){
            Enemy* e = &gs->enemy[i];
            updateAnimation(&e->enemy_animations[e->current_enemy_anim_name],dt);
        }
    }