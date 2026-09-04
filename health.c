#include"health.h"
#include"player.h"
#include"enemy.h"

void updateHealth(GS* gs, float dt){
    if(gs->player.isDead) return;
    gs->player.health -= HEALTH_DECAY_RATE * dt;
    if(gs->player.health <= 0.0f){
        gs->player.health = 0.0f;
        gs->player.isDead = true;
    }
}

void drawHealthUI(GS* gs){
    if(gs->player.isDead) return; // health bar dekhabe an player mara gele
    
    // screen er left e health bar er config gulo
    float barWidth = 200.0f;
    float barHeight = 20.0f;
    float posX = 20.0f;
    float posY = 20.0f;

    // percentage of health
    float healthPercentage = gs->player.health / gs->player.maxHealth;
    if(healthPercentage < 0) healthPercentage = 0.0f; 
        //negative health jeno na hoy

    //bar er pichoner bg dark gray color 
    DrawRectangle(posX, posY, barWidth, barHeight, DARKGRAY);
    

    // percentage er upor depend kore ashot color 
    DrawRectangle(posX, posY, barWidth * healthPercentage, barHeight, RED);

    //chunk e vangtesi health ke 
    int numChunks=1;
    float chunkWidth = barWidth/numChunks;
     for(int i=1;i<numChunks;i++){
        DrawLine(posX + (i*chunkWidth), posY, posX +(i * chunkWidth), posY + barHeight, BLACK);
     }

    // border
    DrawRectangleLines(posX, posY, barWidth, barHeight, BLACK);
  //health percentage 
   int displayPercentage=(int)(healthPercentage*100);

    // health index koto health ase seta  
    DrawText(TextFormat("HP: %d/%d", (int)gs->player.health, (int)gs->player.maxHealth), posX, posY + barHeight + 5, 20, BLACK);

    DrawText(TextFormat("%d%%", displayPercentage), posX, posY + barHeight + 5, 20, WHITE);
}


void damagePlayer(GS* gs,float amount){
    Player* p = &gs->player;
    if(p->isDead || p->invultimer>0) return;
    p->health-=amount;
    p->invultimer = player_invul_time;

    if(p->health<=0.0f){
        p->isDead = true;
        p->health=0.0f;
    }
    else {
        gs->current_player_anim_name = player_hurt;
        p->velocity.x = 0;
    }
}

// void damageEnemy(Enemy *e, float amount) {
//     if (e->isdead) return;

//     e->health -= amount;

//     if (e->health <= 0.0f) {
//         e->health = 0.0f;
//         e->isdead = true;
//         updateEnemyAnimation(e,enemy_dead);
//     } else {
//         updateEnemyAnimation(e,enemy_hurt);
//     }
// }

void updatePlayerInvulnerability(GS* gs,float dt){
    Player* p = &gs->player;
    if (p->invultimer >= 0.0f) {
        p->invultimer -= dt; 
        if (p->invultimer < 0.0f) p->invultimer = 0.0f;
    }
}