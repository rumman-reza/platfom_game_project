#include"health.h"
#include"player.h"

void updateHealth(GS* gs, float dt){
    if(gs->player.isDead) return;
    gs->player.health -= HEALTH_DECAY_RATE * dt;
    if(gs->player.health <= 0.0f){
        gs->player.health = 0.0f;
        gs->player.isDead = true;
        setAnimation(gs,player_die);
    }
}