#include"combat.h"
#include"enemy.h"
#include"player.h"
#include"health.h"

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
                
                if(!p->hashitthiswing && CheckCollisionRecs(player_hitbox,getEnemyRect(en))){
                    damageEnemy(&gs->enemy[i],player_attack_power);                    
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
            if(enemy_attack_start_frame <= e->currentframe && e->currentframe >= enemy_attack_end_frame){
                Rectangle enemy_hitbox = getEnemyHitbox(en);
                if(!en->hashitplayerthisswing && CheckCollisionRecs(getPlayerRect(gs),enemy_hitbox)){
                    damagePlayer(gs,enemy_attack_power);                   
                    en->hashitplayerthisswing=true;                                               // can be changed later
                } 
                else en->hashitplayerthisswing = false;          
            }
        }
    }
}