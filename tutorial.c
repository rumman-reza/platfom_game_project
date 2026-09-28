#include "tutorial.h"
#include "background.h"
#include <string.h>


static const char* tutorialPages[] = {
    "                    THE STORY               \n\n"
    "---------------------------------------------------\n\n"
    "      On your mighty ADVENTURE in the jungle,    \n\n"
    "         You ANGERED the FOREST!!!!\n\n"
    "       AND NOW it's TAKING REVENGE ON YOU\n\n"
    "A POISONOUS GAS CLOUD chases you until DEATH!!\n\n"
    " HOW LONG CAN YOU SURVIVE THIS BONEGROVE?!\n\n\n",


    "          READ THIS VERY CAREFULLY!!!!\n\n"
    "--------------------------------------------\n\n"
    "                   HOW TO PLAY\n\n"
    "                  -------------  \n\n"
    "A / D  -  Move\n\n"
    "SPACE  -  Jump\n\n"
    "LEFT SHIFT  -  Dash ***(really usefull!!)**\n\n"
    "LEFT CLICK  -  Attack\n\n",
    "                     OBSTACLES\n\n"
    "                    -----------\n\n"
    "SPIKES - Touching it causes damage\n\n\n\n\n\n\n\n\n"
    "BOMBS - STAYING IN RANGE CAUSES EXPLOSION\n\n\n",

    "                      ENEMY      \n\n"
    "                    --------\n\n\n\n"
    "ENEMY CHASES YOU AND HITS IF YOU ARE IN RANGE\n\n",

    "                   HEALTH     \n\n"
    "                  --------\n\n\n\n"
    "       collect Health throughout the GAME\n\n\n"
    "       healths randomly spawn on platforms\n\n\n\n\n\n\n\n",

        "                   HEALTH     \n\n"
    "                  ----------\n\n\n\n"
    "      Enemies also drop health when killed\n\n\n\n\n",


   "                    POISON GAS\n\n" 
   "                   -------------\n\n\n"
   "AVOID THE POISION GAS CHASING YOU\n\n\n"
   "Getting close or staying in that will cause DAMAGE\n\n",

   "                ****TIPS******\n\n"
   "                --------------\n\n\n\n"
   "         USE DASH (LEFT SHIFT BUTTON)\n\n"
   "     WHEN THE GAP IS TOO BIG TO JUMP OVER",
   "                ****TIPS******\n\n"
   "                --------------\n\n\n\n"
   "         USE DASH (LEFT SHIFT BUTTON)\n\n"
   "      WHEN THE GAP IS TOO BIG TO JUMP OVER",

   "            USE THE TIPS CORRECTLY\n\n\n\n\n\n\n\n\n"
   "                 !!!!! ENJOY !!!!!\n\n\n\n\n\n\n"
    "                OR SHOULD YOU?"
};
static const int tutorialPageCount = sizeof(tutorialPages)/sizeof(tutorialPages[0]);

static void startPage(GS* gs, int page){
    gs->tutorial_page = page;
    gs->tutorial_charsShown = 0;
    gs->tutorial_charTimer = 0.0f;
    gs->tutorial_alpha = 0.0f;
    gs->tutorial_state = tut_fadein;
}

void initTutorial(GS* gs){
    startPage(gs, 0);
}

void updateTutorial(GS* gs, float dt){
    int textLen = (int)strlen(tutorialPages[gs->tutorial_page]);
    
       if(!gs->pressed_how_to_play){
       if(IsKeyDown(KEY_ENTER)){
           gs->skip_pressed_timer += dt;
           if(gs->skip_pressed_timer >= gs->skip_duration){
               gs->show_tutorial = false;
               gs->skip_pressed_timer = 0;
               gs->currentscreen = GAME;
               return;
           }
       } else gs->skip_pressed_timer = 0;
   }
    
    switch(gs->tutorial_state){
        case tut_fadein:
            gs->tutorial_alpha += tutorial_fade_speed*dt;
            if(gs->tutorial_alpha >= 1.0f){
                gs->tutorial_alpha = 1.0f;
                gs->tutorial_state = tut_typing;
            }
            break;

        case tut_typing:
            gs->tutorial_charTimer += dt;
            while(gs->tutorial_charTimer >= tutorial_char_interval && gs->tutorial_charsShown < textLen){
                gs->tutorial_charTimer -= tutorial_char_interval;
                gs->tutorial_charsShown++;
            }
            if((IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) && gs->tutorial_charsShown < textLen){
                gs->tutorial_charsShown = textLen;  
                PlaySound(gs->audio.menu_click);
            } else if(gs->tutorial_charsShown >= textLen){
                gs->tutorial_state = tut_waiting;
            }
            break;

        case tut_waiting:
            if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)){
                gs->tutorial_state = tut_fadeout;
            }
            break;

        case tut_fadeout:
            gs->tutorial_alpha -= tutorial_fade_speed*dt;
            if(gs->tutorial_alpha <= 0.0f){
                gs->tutorial_alpha = 0.0f;
                if(gs->tutorial_page + 1 < tutorialPageCount){
                    startPage(gs, gs->tutorial_page + 1);
                } else {
                    gs->show_tutorial = false;
                    if(gs->pressed_how_to_play) {gs->currentscreen = MENU; gs->pressed_how_to_play = false;}
                    else gs->currentscreen = GAME;
                    gs->show_tutorial = false;
                }
            }
            break;
    }

    updateParallax(gs, 5.0f);   // same subtle background drift as your menu
}

void drawTutorial(GS* gs,tex* tex){
    drawBackgroundMenu(gs);
    DrawRectangle(0,0,s_width,s_height,GetColor(0x000000AA));
    DrawRectangle(0,0,s_width,s_height,GetColor(0x00000088));
    const char* text = tutorialPages[gs->tutorial_page];
    int n = gs->tutorial_charsShown;

    char visible[1024];
    if(n >= (int)sizeof(visible)) n = sizeof(visible)-1;
    memcpy(visible, text, n);
    visible[n] = '\0';

    unsigned char a = (unsigned char)(gs->tutorial_alpha * 255);
    Color textColor = (Color){255,255,255,a};

    // manual line-splitting: DrawTextEx doesn't wrap embedded newlines on its own
    float y = 150.0f;
    float lineHeight = 34.0f;
    char lineBuf[256];
    int lineStart = 0;
    int len = (int)strlen(visible);

    for(int i=0; i<=len; i++){
        if(visible[i]=='\n' || visible[i]=='\0'){
            int lineLen = i - lineStart;
            if(lineLen >= (int)sizeof(lineBuf)) lineLen = sizeof(lineBuf)-1;
            memcpy(lineBuf, visible+lineStart, lineLen);
            lineBuf[lineLen] = '\0';
            Vector2 size = MeasureTextEx(gs->cfonts.menu_font3, lineBuf, 28, 0);
            DrawTextEx(gs->cfonts.menu_font3, lineBuf, (Vector2){380, y}, 50, 0, textColor);
            y += lineHeight;
            lineStart = i+1;
        }
    }

    if(gs->tutorial_state == tut_waiting){
        const char* prompt = (gs->tutorial_page+1 < tutorialPageCount) ? "Press ENTER to continue " : "Press ENTER to start";
        if((int)(GetTime()*2) % 2 == 0){
            Vector2 psize = MeasureTextEx(gs->cfonts.menu_font3, prompt, 20, 0);
            DrawTextEx(gs->cfonts.menu_font3, prompt, (Vector2){700, s_height-100}, 40, 0, Fade(LIGHTGRAY, gs->tutorial_alpha));
        }
    }

    if(gs->tutorial_page==2){
        float width = tex->bomb_sprite.width;
        float height = tex->bomb_sprite.height;
        float player_posx = s_width/2.0f-250.0f;
        float player_posy = s_height/2.0f+200.0f;
        if(gs->tutorial_state == tut_waiting)DrawTexturePro(
            tex->bomb_sprite,(Rectangle){.x =0,.y=0,.height = height, .width = width}, 
            (Rectangle){.x = player_posx+80.0f,.y = player_posy-60.0f,.width = width/1.5f, .height = height/1.5f},
            (Vector2){0,0},0,WHITE
        );

        float width2 = tex->spike_sprite.width/4.0f;
        float height2 = tex->spike_sprite.height;
         if(gs->tutorial_state == tut_waiting) DrawTexturePro(
            tex->spike_sprite,(Rectangle){.x =0,.y=0,.height = height2, .width = width2}, 
            (Rectangle){.x = player_posx+80.0f,.y = player_posy-400.0f,.width = width2*SPRITE_SCALE*4.0f, .height= height2*SPRITE_SCALE*4.0f},
            (Vector2){0,0},0,WHITE
        );
    }
    if(gs->tutorial_page==3){
        float player_posx = s_width/2.0f-250.0f;
        float player_posy = s_height/2.0f+200.0f;
        float width3 = tex->enemy_attack.width/18.0f;
        float height3  = tex->enemy_attack.height;
        if(gs->tutorial_state == tut_waiting) DrawTexturePro(
            tex->enemy_attack,(Rectangle){.x = 5*width3,.y=0,.height = height3, .width = width3}, 
            (Rectangle){.x = player_posx+160.0f,.y = player_posy-240.0f,.width = width3*SPRITE_SCALE*2.5f, .height = height3*SPRITE_SCALE*2.5f},
            (Vector2){0,0},0,WHITE
        );
    }
    if(gs->tutorial_page==4){
        float player_posx = s_width/2.0f-250.0f;
        float player_posy = s_height/2.0f+200.0f;
        float width3 = tex->platform_health_drop.width;
        float height3  = tex->platform_health_drop.height;
        if(gs->tutorial_state == tut_waiting) DrawTexturePro(
            tex->platform_health_drop,(Rectangle){.x = 0,.y=0,.height = height3, .width = width3}, 
            (Rectangle){.x = player_posx-30,.y = player_posy-230.0f,.width = width3/1.6f, .height = height3/1.6f},
            (Vector2){0,0},0,WHITE
        );
    }
    if(gs->tutorial_page==5){
        float player_posx = s_width/2.0f-250.0f;
        float player_posy = s_height/2.0f+200.0f;
        float width3 = tex->enemy_health_drop.width;
        float height3  = tex->enemy_health_drop.height;
        if(gs->tutorial_state == tut_waiting) DrawTexturePro(
            tex->enemy_health_drop,(Rectangle){.x = 5*width3,.y=0,.height = height3, .width = width3}, 
            (Rectangle){.x = player_posx+120,.y = player_posy-240.0f,.width = width3, .height = height3},
            (Vector2){0,0},0,WHITE
        );
    }
    if(gs->tutorial_page==6){
        float player_posx = s_width/2.0f-250.0f;
        float player_posy = s_height/2.0f+200.0f;
        float width3 = tex->pgas.width;
        float height3  = tex->pgas.height;
        if(gs->tutorial_state == tut_waiting) DrawTexturePro(
            tex->pgas,(Rectangle){.x = 5*width3,.y=0,.height = height3, .width = width3}, 
            (Rectangle){.x = player_posx-60,.y = player_posy-260.0f,.width = width3/2.0f, .height = height3/2.0f},
            (Vector2){0,0},0,WHITE
        );
    }
     if(gs->tutorial_page==7){
        float player_posx = s_width/2.0f-250.0f;
        float player_posy = s_height/2.0f+200.0f;
        float width3 = tex->Large_gap.width;
        float height3  = tex->Large_gap.height;
        if(gs->tutorial_state == tut_waiting) DrawTexturePro(
            tex->Large_gap,(Rectangle){.x = 5*width3,.y=0,.height = height3, .width = width3}, 
            (Rectangle){.x = player_posx-60,.y = player_posy-200.0f,.width = width3/2.0f, .height = height3/2.0f},
            (Vector2){0,0},0,WHITE
        );
    }
     if(gs->tutorial_page==8){
        float player_posx = s_width/2.0f-250.0f;
        float player_posy = s_height/2.0f+200.0f;
        float width3 = tex->Large_gap2.width;
        float height3  = tex->Large_gap2.height;
        if(gs->tutorial_state == tut_waiting) DrawTexturePro(
            tex->Large_gap2,(Rectangle){.x = 5*width3,.y=0,.height = height3, .width = width3}, 
            (Rectangle){.x = player_posx-60,.y = player_posy-200.0f,.width = width3/2.0f, .height = height3/2.0f},
            (Vector2){0,0},0,WHITE
        );
    }
}