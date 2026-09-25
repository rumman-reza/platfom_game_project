#include"enemy.h"
#include"animation.h"
#include<math.h>
#include"player.h"
#include"raymath.h"
#include"health.h"
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
    en_run->frameduration = .04f;
    en_run->frameHeight = tex->enemy_run.height;
    en_run->frameWidth = tex->enemy_run.width/en_run->framecount;
    en_run->looping = true;
    en_run->timedependent = true;
    en_run->isfinished = false;

    anim* en_dead = &enemy.enemy_animations[enemy_dead];
    en_dead->tex = tex->enemy_dead;
    en_dead->framecount = 15;
    en_dead->frameduration = .1f;
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

    
    enemy.height = en_idle->frameHeight * SPRITE_SCALE * 1.8f;
    enemy.width  = en_idle->frameWidth  * SPRITE_SCALE * 1.8f;
    enemy.hashitplayerthisswing = false;
    enemy.attack_cooldown = 0.0f;
    enemy.invultimer = 0.0f;
    enemy.state = walking_enemy;
    enemy.health = enemy_max_health;

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


static void restartEnemyAnim(Enemy* e, enemy_anim n){
    e->current_enemy_anim_name = n;
    anim* a = &e->enemy_animations[n];
    a->currentframe = 0;
    a->frametimer = 0.0f;
    a->isfinished = false;
}

static void startEnemyAttack(GS*gs, Enemy* e){
    PlaySound(gs->audio.enemy_swing);
    e->state = attacking_enemy;
    e->velocity.x = 0;
    e->hashitplayerthisswing = false;
    e->attack_cooldown = encooldown;   
    restartEnemyAnim(e, enemy_attack);
}

void updateEnemy(GS* gs,float dt){
    float left_edge = gs->camera.target.x - s_width/2.0f;
    bool player_alive = !gs->player.isDead;

    for(int i=0;i<max_enemy_num;i++){
        Enemy* enemy = &gs->enemy[i];
        if(!enemy->isactive) continue;

        // far behind the camera: free the slot
        if(getEnemyRect(enemy).x + enemy->width < left_edge - enemy_despawn_margin){
            enemy->isactive = false;
            continue;
        }

        float player_center_x = getPlayerCenterX(gs);
        float enemy_center_x  = enemy->position.x+enemy->width/2.0f;
        float dist = fabsf(player_center_x - enemy_center_x);

        // don't turn around mid-swing or while dying
        if(enemy->state != attacking_enemy && enemy->state != dead_enemy)
            enemy->facing_left = (player_center_x < enemy_center_x);

        if(enemy->attack_cooldown > 0) enemy->attack_cooldown -= dt;
        anim* a = &enemy->enemy_animations[enemy->current_enemy_anim_name];

        // player dead: chasing/attacking enemies calm down (no frozen mid-swing pose)
        if(!player_alive && (enemy->state == walking_enemy || enemy->state == attacking_enemy)){
            enemy->state = idle_enemy;
            restartEnemyAnim(enemy, enemy_idle);
        }

        switch(enemy->state){
            case idle_enemy:{
                if(dist <= enemy_aggro_range && player_alive){
                    enemy->state = walking_enemy;
                    restartEnemyAnim(enemy, enemy_running);
                }
                break;
            }
            case dead_enemy:{
                enemy->velocity.x = 0;
                if(a->isfinished){
                    enemy->isactive = false;
                    enemy->isdead = true;
                }
                break;
            }
            case hurting_enemy:{
                if(enemy->current_enemy_anim_name == enemy_hurt && !a->isfinished) break; // still stunned
                if(dist <= attackrange && player_alive) startEnemyAttack(gs,enemy);
                else{
                    enemy->state = walking_enemy;
                    updateEnemyAnimation(enemy, enemy_running);
                }
                break;
            }
            case walking_enemy:{
                if(dist <= attackrange){
                    startEnemyAttack(gs,enemy);
                    break;
                }
                float dir = enemy->facing_left ? -1.0f : 1.0f;
                enemy->velocity.x = dir * enSpeed;
                enemy->position.x += enemy->velocity.x * dt;
                updateEnemyAnimation(enemy, enemy_running);
                break;
            }
            case attacking_enemy:{
                enemy->velocity.x = 0;
                if(a->isfinished && dist > attackrange){ 
                    enemy->state = walking_enemy;
                    updateEnemyAnimation(enemy, enemy_running);
                }
                else if(enemy->attack_cooldown <= 0 && dist <= attackrange){
                    startEnemyAttack(gs,enemy);    
                }
                break;
            }
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

void damageEnemy(GS* gs,Enemy* e,float amount){
    if (e->isdead || e->invultimer>0.0f) return;
    e->health -= amount;
    e->invultimer = enemy_invultimer;

    if (e->health <= 0.0f) {
        e->health = 0.0f;
        e->isdead = true;
        e->state = dead_enemy;
         PlaySound(gs->audio.enemyDie);
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

void move_pgas(GS* gs,float dt){
    float distance = (gs->player.position.x+gs->player.width/2.0f-gs->pgas.position.x-gs->pgas.pgas_anim[0].width/2.0f+40.0f);
    if(0>distance){
        gs->pgas.velocity.x = 0.0f;
        if(gs->player.velocity.x<0){
            gs->pgas.position.x = gs->player.position.x+gs->player.width/2.0f+40.0f;
        }
        else gs->pgas.position.x = gs->pgas.position.x;
    }
    else gs->pgas.velocity.x = 400.0f;

    gs->pgas.position = Vector2Add(gs->pgas.position,Vector2Scale(gs->pgas.velocity,dt));

}

Rectangle getPgasRect(GS* gs){
    return (Rectangle){
        .x = gs->pgas.position.x,
        .y = gs->pgas.position.y,
        .width = gs->pgas.pgas_anim[0].width,
        .height = gs->pgas.pgas_anim[0].height
    };
}

void drawPgasSprite(GS* gs){
    Rectangle source = (Rectangle){
        .x = 0,
        .y = 0,
        .width = (gs->player.facing_left)?-gs->pgas.pgas_anim[gs->pgas.current_texture].width:gs->pgas.pgas_anim[gs->pgas.current_texture].width,
        .height = gs->pgas.pgas_anim[gs->pgas.current_texture].height
    };
    Rectangle dest = (Rectangle){
        .x = gs->pgas.position.x,
        .y = gs->pgas.position.y,
        .width = gs->pgas.pgas_anim[gs->pgas.current_texture].width,
        .height = gs->pgas.pgas_anim[gs->pgas.current_texture].height
    };
    DrawTexturePro(gs->pgas.pgas_anim[gs->pgas.current_texture],source,dest,(Vector2){0,0},0,WHITE);

}

void DamageFromSpikes(GS* gs,float dt){

    gs->spike_cooldown-=dt;
    if(gs->spike_cooldown<0) gs->spike_cooldown = 0;
    for(int i = 0; i < max_spikes; i++){
        if(!gs->spikes[i].isactive) continue;

        if(CheckCollisionRecs(getPlayerRect(gs), gs->spikes[i].rect)){
            if(gs->spike_cooldown==0) damagePlayer(gs,spike_damage);
            gs->spike_cooldown = spikecooldown;
        }   

    }
}
void spawnEnemy(GS* gs, float x, float groundY){
    for(int i = 0; i < max_enemy_num; i++){
        Enemy* e = &gs->enemy[i];
        if(e->isactive) continue;

        e->position = (Vector2){ x, groundY - e->height };
        e->velocity = (Vector2){0,0};
        e->health = enemy_max_health;
        e->isdead = false;
        e->invultimer = 0.0f;
        e->attack_cooldown = 0.0f;
        e->hashitplayerthisswing = false;
        e->state = idle_enemy;
        restartEnemyAnim(e, enemy_idle);
        e->isactive = true;
        return; // one spawn per call — pattern.c calls this once per 'E' tile
    }
    TraceLog(LOG_WARNING,"spawnEnemy: no inactive slot free (max_enemy_num=%d)",max_enemy_num);
}


void initFogPuffs(GS* gs){
    for(int i=0;i<25;i++){
        gs->fogpuffs[i].offset.x = -(float)GetRandomValue(0, 1600);
        gs->fogpuffs[i].offset.y = (float)GetRandomValue((int)fog_top_gap, s_height - (int)fog_bottom_gap);
        gs->fogpuffs[i].radius = (float)GetRandomValue(40, 110);
        gs->fogpuffs[i].speed = (float)GetRandomValue(10, 30)/10.0f;
        gs->fogpuffs[i].phase = (float)GetRandomValue(0, 628)/100.0f;
    }
}

// draws one vertical strip of fog at world-x `x`, width `w`, faded at top/bottom,
// scaled by alphaScale (0..1) so callers can fade it horizontally too
static void drawFogStrip(float x, float w, float alphaScale){
    float topY = fog_top_gap;
    float bottomY = s_height - fog_bottom_gap;
    float midY1 = topY + fog_vfade;
    float midY2 = bottomY - fog_vfade;

    unsigned char a = (unsigned char)(fog_base_alpha * alphaScale);
    Color solid = (Color){fog_color_r, fog_color_g, fog_color_b, a};
    Color clear = (Color){fog_color_r, fog_color_g, fog_color_b, 0};

    DrawRectangleGradientV((int)x,(int)topY,(int)w,(int)fog_vfade, clear, solid);         // fades in from top
    DrawRectangle((int)x,(int)midY1,(int)w,(int)(midY2-midY1), solid);                    // solid middle band
    DrawRectangleGradientV((int)x,(int)midY2,(int)w,(int)fog_vfade, solid, clear);        // fades out at bottom
}

void drawPgasFill(GS* gs){
    float leftEdge = gs->camera.target.x - gs->camera.offset.x;
    float edgeX = gs->pgas.position.x + gs->pgas.pgas_anim[0].width/2.0f;
    float solidRight = edgeX - fog_edge_fade_width;

    if(solidRight > leftEdge){
        drawFogStrip(leftEdge, solidRight - leftEdge, 1.0f);
    }
}

void drawPgasEdgeFade(GS* gs){
    float edgeX = gs->pgas.position.x + gs->pgas.pgas_anim[0].width/2.0f;
    float stripW = fog_edge_fade_width / fog_edge_strips;

    for(int i=0;i<fog_edge_strips;i++){
        float t = (float)i / fog_edge_strips;      // 0 = deep in fog, 1 = clear air
        float x = edgeX - fog_edge_fade_width + i*stripW;
        drawFogStrip(x, stripW+2, 1.0f - t);
    }
}

void drawFogPuffs(GS* gs, float time){
    float edgeX = gs->pgas.position.x + gs->pgas.pgas_anim[0].width/2.0f;
    for(int i=0;i<25;i++){
        float x = edgeX + gs->fogpuffs[i].offset.x;
        float y = gs->fogpuffs[i].offset.y + sinf(time*gs->fogpuffs[i].speed + gs->fogpuffs[i].phase)*15.0f;
        DrawCircleGradient((Vector2){(int)x,(int)y},gs->fogpuffs[i].radius,(Color){60,120,60,90},(Color){60,120,60,0});
    }
}