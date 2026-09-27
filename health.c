#include"health.h"
#include"player.h"
#include"enemy.h"
#include<string.h>
#include<stdio.h>
#include<math.h>

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
    DrawTextEx(gs->cfonts.menu_font3, heroname, (Vector2){posX + 3, posY + 3}, 60, 0, Fade(BLACK, 0.6f)); 

    DrawTextEx(gs->cfonts.menu_font3, heroname, (Vector2){posX, posY}, 60, 0, GOLD); 

    float barY = posY + 70.0f;
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
    const char* hpText = TextFormat("HP: %d / %d", (int)gs->player.health, (int)gs->player.maxHealth);
    
    DrawTextEx(gs->cfonts.menu_font3,hpText, (Vector2){posX + 5, barY + barHeight + 8}, 30, 0,RAYWHITE);
}


void damagePlayer(GS* gs,float amount){
    Player* p = &gs->player;
    if(p->isDead || p->invultimer>0) return;
    p->health-=amount;
    spawn_health_update(gs,-amount);
    p->invultimer = player_invul_time;

    if(p->health<=0.0f){
        p->isDead = true;
        p->health=0.0f;
        PlaySound(gs->audio.die);
    }
    else {
        gs->current_player_anim_name = player_hurt;
        p->velocity.x = 0;
         PlaySound(gs->audio.hurt);
    }
}


void updatePlayerInvulnerability(GS* gs,float dt){
    Player* p = &gs->player;
    if (p->invultimer >= 0.0f) {
        p->invultimer -= dt; 
        if (p->invultimer < 0.0f) p->invultimer = 0.0f;
    }
}

void spawn_health_update(GS* gs,int amount){
    Rectangle playerRect = getPlayerRect(gs);
    for(int t=0;t<10;t++){
        if(!gs->floatTexts[t].active){
            gs->floatTexts[t].active = true;
            
            gs->floatTexts[t].position =(Vector2){playerRect.x,playerRect.y-30.0f};
            gs->floatTexts[t].timer=1.5f;
            //1.5 ssec er jonno screen ee tahkbe 
            gs->floatTexts[t].maxTime=1.5f;
            char show[40];
            sprintf(show,"%+d %s",amount, "HP");
            strcpy(gs->floatTexts[t].text, show);
            gs->floatTexts[t].color =(amount<0)?RED:GOLD;
            break;  
            
        }
    }
}

void spawnHealthDrop(GS* gs, float x, float y){
    int index = gs->healthDrop_index;

    gs->healthDrops[index].rect = (Rectangle){
        .x = x - health_drop_width/2.0f,
        .y = y - health_drop_height/2.0f,
        .width = health_drop_width,
        .height = health_drop_height
    };
    gs->healthDrops[index].healAmount = enemy_health_drop_amount;
    gs->healthDrops[index].active = true;

    gs->healthDrop_index = (gs->healthDrop_index + 1) % max_health_drops;
}

void updateHealthDropPickup(GS* gs){
    if(gs->player.isDead) return;
    Rectangle playerRect = getPlayerRect(gs);

    for(int i=0;i<max_health_drops;i++){
        HealthDrop* d = &gs->healthDrops[i];
        if(!d->active) continue;

        if(CheckCollisionRecs(playerRect, d->rect)){
            gs->player.health += d->healAmount;
            if(gs->player.health > gs->player.maxHealth) gs->player.health = gs->player.maxHealth;

            PlaySound(gs->audio.health_pickup);
            spawn_health_update(gs, (int)d->healAmount);
            d->active = false;
        }
    }
}

void drawHealthDrops(GS* gs, tex* textures){
    for(int i=0;i<max_health_drops;i++){
        HealthDrop* d = &gs->healthDrops[i];
        if(!d->active) continue;

        Rectangle source = {0, 0, textures->health_item.width, textures->health_item.height};
        float floatOffset = 3.0f*sinf(GetTime()*6.0f)*8.0f;   // same bob effect as your ground health items

        Rectangle dest = {
            .x = d->rect.x,
            .y = d->rect.y + floatOffset,
            .width = d->rect.width,
            .height = d->rect.height
        };
        DrawTexturePro(textures->health_item, source, dest, (Vector2){0,0}, 0.0f, WHITE);
    }
}