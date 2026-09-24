#include"health.h"
#include"player.h"
#include"enemy.h"

void updateHealth(GS* gs, float  dt){
    if(gs->player.isDead) return;
    // gs->player.health -= HEALTH_DECAY_RATE * dt;
    if(gs->player.health <= 0.0f){
        gs->player.health = 0.0f;
        gs->player.isDead = true;
    }
}

void drawHealthUI(GS* gs){
    if(gs->player.isDead) return;

    float posX = 30.0f;
    float posY = 30.0f;


    const char* heroname = TextFormat("%s", gs->playerName); 
    //shadow
    DrawTextEx(gs->cfonts.menu_font3, heroname, (Vector2){posX + 3, posY + 3}, 40, 0, Fade(BLACK, 0.6f)); 

    DrawTextEx(gs->cfonts.menu_font3, heroname, (Vector2){posX, posY}, 40, 0, GOLD); 

    float barY = posY + 45.0f;
    float barWidth = 250.0f; 
    float barHeight = 25.0f;
    
    float healthPercentage = gs->player.health / gs->player.maxHealth;
    if(healthPercentage < 0) healthPercentage = 0.0f; 

    DrawRectangleRounded((Rectangle){posX, barY, barWidth, barHeight}, 0.5f, 10, Fade(BLACK, 0.7f));
    
    Color healthColor = LIME;
    if(healthPercentage <= 0.5f) healthColor = YELLOW;
    if(healthPercentage <= 0.25f) healthColor = RED;

    DrawRectangleRounded((Rectangle){posX, barY, barWidth * healthPercentage, barHeight}, 0.5f, 10, healthColor);

    DrawRectangleRoundedLines((Rectangle){posX, barY, barWidth, barHeight}, 0.5f, 10, LIGHTGRAY);

    int displayPercentage = (int)(healthPercentage * 100);
    const char* hpText = TextFormat("HP: %d / %d  (%d%%)", (int)gs->player.health, (int)gs->player.maxHealth, displayPercentage);
    
    DrawText(hpText, posX + 5, barY + barHeight + 8, 20, RAYWHITE);
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


void updatePlayerInvulnerability(GS* gs,float dt){
    Player* p = &gs->player;
    if (p->invultimer >= 0.0f) {
        p->invultimer -= dt; 
        if (p->invultimer < 0.0f) p->invultimer = 0.0f;
    }
}