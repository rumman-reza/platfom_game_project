#include"combat.h"
#include"enemy.h"
#include"player.h"
#include"health.h"
#include"explosion.h"
#include"raymath.h"

void updateCombat(GS *gs, float dt){
    Player* p =  &gs->player;
    anim* a = &gs->player_animations[gs->current_player_anim_name];

    // ==---player er jono--===
    if(p->isattacking){
        p->hitduration-=dt;
        if(attackstartframe<= a->currentframe && a->currentframe <= attackendframe){
            // check collision of hitbox with enemy rectangle then decrease enemy health
            for(int i=0;i<max_enemy_num;i++){

                Enemy* en = &gs->enemy[i];

                if(!en->isactive) continue; //jodi more jai then kichu korar dorkar nai

                Rectangle player_hitbox = getplayerhitbox(gs);
                
                if( CheckCollisionRecs(player_hitbox,getEnemyRect(en))){
                    damageEnemy(gs,&gs->enemy[i],player_attack_power);                    
                    if(!p->hashitthiswing)PlaySound(gs->audio.hit);
                    p->hashitthiswing = true; //jodi ekbare shudhu ekta enemy ke attack korte pare tahole                                               // can be changed later
                } 
            }
        }
        if(p->hitduration<=0){
            p->isattacking = false;
            p->hashitthiswing = false; 
        }
    } 
    
    // --enemy er jonno--
    for(int i=0;i<max_enemy_num;i++){
        Enemy* en = &gs->enemy[i];
        anim* e = &en->enemy_animations[en->current_enemy_anim_name];
        if(en->isdead || !en->isactive) continue;
        if(en->state == attacking_enemy && en->current_enemy_anim_name == enemy_attack){
            if(enemy_attack_start_frame <= e->currentframe && e->currentframe <= enemy_attack_end_frame){
                Rectangle enemy_hitbox = getEnemyHitbox(en);
                if(!en->hashitplayerthisswing && CheckCollisionRecs(getPlayerRect(gs),enemy_hitbox)){
                    damagePlayer(gs,enemy_attack_power);                   
                    en->hashitplayerthisswing=true;    
                }          
            }
        }
    }
    // pgas er jonno
    gs->pgas.attacktimer-=dt;
    if(gs->pgas.attacktimer<=0) gs->pgas.attacktimer=0;
    if(CheckCollisionRecs(getPlayerRect(gs),getPgasRect(gs)) && gs->pgas.attacktimer==0){
        damagePlayer(gs,gs->pgas.pgas_damage);
        gs->pgas.attacktimer = gs->pgas.attackcooldown;
    }
}


void DamageFromBombs(GS* gs, float dt) {
    Rectangle playerRect = getPlayerRect(gs);
    Vector2 playerCenter = {
        playerRect.x + playerRect.width/2.0f,
        playerRect.y + playerRect.height/2.0f
    };

    for (int i = 0; i < max_bombs; i++) {
        bomb* b = &gs->bombs[i];
        if (!b->isactive) continue;

        Vector2 bombCenter = {
            b->rect.x + b->rect.width/2.0f,
            b->rect.y + b->rect.height/2.0f
        };
        bool playerInRange = Vector2Distance(playerCenter, bombCenter) <= bomb_explosion_range;

        if (playerInRange) {
            if (!b->armed) {
                b->armed = true;
                b->fuseTimer = bomb_fuse_time;
            } else {
                b->fuseTimer -= dt;
            }

            if (b->fuseTimer <= 0.0f) {
                b->isactive = false;
                spawnExplosion(gs, bombCenter);
                PlaySound(gs->audio.explosion);
                damagePlayer(gs, bomb_damage);   // player is still in range at this instant, by definition
            }
        } else {
            // player got out before it went off: disarm, so re-entering later starts a fresh fuse
            b->armed = false;
            b->fuseTimer = 0.0f;
        }
    }
}