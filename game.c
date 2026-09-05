    #include "game.h"
    #include "texture.h"
    #include "animation.h"
    #include "player.h"
    #include "enemy.h"
    #include "ground.h"
    #include "background.h"
    #include "camera.h"
    #include "health.h"
    #include"combat.h"
    void drawGame(GS* gs){
    
    //drawing background elements

    drawBackground(gs);
    DrawRectangle(gs->camera.target.x-s_width,0,2*s_width,s_height,GetColor(0x00000060));

    //drawing the ground rectangles;
    for(int i=0;i<MaxChunkNum;i++){
        DrawRectangleRec(gs->gchunk[i].groundChunkRect,DARKBROWN);
        // DrawRectangleLinesEx(gs->gchunk[i].groundChunkRect,3,BLACK);
        
        
        // ei chunk e heal item thakle  and seta pick na kore
        //thakle box ta green 
        if(gs->gchunk[i].hasHealthItem && !gs->gchunk[i].healthItemCollected){
            DrawRectangleRec(gs->gchunk[i].healthItemRect, GREEN);
        }
    }    
    //drawing player sprite

        drawPlayerSprite(gs);
        // DrawRectangleLinesEx(getplayerhitbox(gs),20,BLACK);
        // DrawRectangleLinesEx(getPlayerRect(gs),10,(gs->player.isattacking)?RED:BLUE);

    //drawing enemy sprites
        for(int i=0;i<max_enemy_num;i++){
            drawEnemy(&gs->enemy[i]);
            // DrawRectangleLinesEx(getEnemyHitbox(&gs->enemy[i]),20,BLACK);
            // DrawRectangleLinesEx(getEnemyRect(&gs->enemy[i]),10,BLUE);
        }
    }

    void initGame(GS* gs,tex* tex,anim* anim){

        float ground_y = s_height*3.7f/4;
        float ground_height = s_height-ground_y;
        SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);

    // load textures
        loadTexture(tex,gs);
    // load animations
        loadAnimation(gs,tex,anim);

        //menu 
        gs->currentscreen = MENU;
        gs->menu_selection = 0; //1st e start game e select hoye tahkbe 
        gs->quit_game = false;
        
        //set player
        float scale = SPRITE_SCALE * 1.6f;
        float frameW = gs->player_animations[player_idle].frameWidth;   // 80
        float frameH = gs->player_animations[player_idle].frameHeight;  // 48


        gs->player.width  = player_real_width  * scale;   // 17 * scale
        gs->player.height = player_real_height * scale;   // 32 * scale

        gs->player.collisionOffset.x = 30.0f * scale;
        gs->player.collisionOffset.y = 16.0f * scale;

        gs->player.initial_position.x = s_width/2.0f - (frameW * scale) / 2.0f;  
        gs->player.initial_position.y = ground_y - (frameH * scale);            

        gs->player.position = gs->player.initial_position;


    //set camera 
        gs->camera.offset = (Vector2){s_width/2.0f,0.0f};
        gs->camera.rotation = 0.0f;
        gs->camera.zoom = 1.0f;

        float player_center_x = gs->player.position.x + gs->player.collisionOffset.x + gs->player.width / 2.0f;
        gs->camera.target = (Vector2){player_center_x-camera_half_deadzone,0.0f};
        gs->last_camera_x = gs->camera.target.x;
    // setup initial Ground
        gs->next_spawn_point=-s_width;
        gs->chunk_index=0;

    //heath function er variable gulo
        gs->player.maxHealth = PLAYER_MAX_HEALTH;
        gs->player.health = PLAYER_MAX_HEALTH;
        gs->player.isDead = false;


    for(int i=0;i<MaxChunkNum;i++){
            gs->gchunk[i].groundChunkRect = (Rectangle){gs->next_spawn_point,ground_y,s_width,ground_height};
            gs->gchunk[i].hasHealthItem = true; 
            gs->gchunk[i].healthItemCollected = false;
            gs->gchunk[i].healthItemRect = (Rectangle){
                .x = gs->next_spawn_point + (s_width * 0.8f),
                .y = ground_y - 40.0f,
                .width = 30.0f,
                .height = 30.0f
            };
            
            
            gs->next_spawn_point+=s_width;
        }
        // setup background layers — farthest (slowest apparent motion) to nearest
        float bg_scrollfactors[BG_LAYER_COUNT] = {0.1f, 0.25f, 0.45f,0.65f , 0.85f,.95f};
        
        for(int i=0;i<BG_LAYER_COUNT;i++){
            parallax_layer *l = &gs->bgLayers[i];
            l->scrollfactor = bg_scrollfactors[i];
            l->offsetX = 0.0f;
            l->source = (Rectangle){
                .x = 0, .y = 0,
                .width  = l->tex.width,
                .height = l->tex.height
            };
        }
        // spawing a random enemy
        for(int i=0;i<max_enemy_num;i++){
            gs->enemy[i] = loadEnemy(tex);
        }
    }
    void unloadenemy(GS* gs){
        for(int i=0;i<max_enemy_num;i++){
            UnloadEnemyAnims(&gs->enemy[i]); 
        }
    }

    void updateGameplay(GS* gs,anim* anim,float dt){
        playerDashUpdate(gs,dt);
        Gravity(gs,dt);
        hitting(gs,dt);
        playerMovement(gs,anim,dt);
        restrict_left_movement(gs);
        groundedCheck(gs,dt);
        setplayerstate(gs);
        updateJumpFrame(gs);
        updateAnimation(&gs->player_animations[gs->current_player_anim_name],dt);
        
        updateHealth(gs,dt);
        checkHealthPickup(gs); // collision ditect check 

        updateGround(gs);
        cameraMovement(gs);

        float cameradelta = gs->camera.target.x - gs->last_camera_x;
        updateParallax(gs,cameradelta);
        gs->last_camera_x = gs->camera.target.x;


        //enemy functions
        updateEnemyInvultimer(gs,dt);
        updatePlayerInvulnerability(gs,dt);
        updateCombat(gs,dt);
        updateEnemy(gs,dt);
        updateEnemyAnimations(gs,dt);
        isGameover(gs,dt);

    }


void updateGame(GS* gs, anim* anim, float dt){
    switch(gs->currentscreen){
        case MENU: 
            updateMenu(gs); 
            break;
        case NAME_ENTRY: 
            updateNameEntry(gs); 
            break;
        case GAME: 
            updateGameplay(gs, anim, dt); 
            break;
        case GAMEOVER:
            break;
    }
}



//menu functions

void updateMenu(GS* gs) {
   //down key niche toggle korar jonno 
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        gs->menu_selection++;
        if (gs->menu_selection > 1) gs->menu_selection = 0; // 2 tar besi option nai tao 0 te chole jabe 
    }
    //up key te vice versa
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        gs->menu_selection--;
        if (gs->menu_selection < 0) gs->menu_selection = 1;
    }

    //enter key
    if (IsKeyPressed(KEY_ENTER)) {
        if (gs->menu_selection == 0) {
            gs->currentscreen = NAME_ENTRY; // name page 
            gs->nameLetterCount = 0;        // name reset kora
            gs->playerName[0] = '\0';
        } 
        else if (gs->menu_selection == 1) {
            gs->quit_game = true;
        }
    }
    updateParallax(gs,5.0f);
}

void drawMenu(GS* gs) {
    drawBackgroundMenu(gs);
    DrawRectangle(0,0,s_width,s_height,GetColor(0x000000AA));
    //menu title --epic adv
    const char* title = "EPIC ADVENTURE";
    int titleWidth = MeasureText(title, 80);
    
    // shadow ar main text 
    DrawTextEx(gs->cfonts.menu_font1,title,(Vector2){(s_width / 2) -360- (titleWidth / 2) + 5, s_height / 4 + 5}, 150, 0,BLACK);
    DrawTextEx(gs->cfonts.menu_font1,title,(Vector2){ (s_width / 2) -360- (titleWidth / 2), s_height / 4}, 150,0, GOLD);

    //option selected jeta seta lal dekahbe 
    Color startColor = (gs->menu_selection == 0) ? WHITE : DARKGRAY;
    Color exitColor  = (gs->menu_selection == 1) ? WHITE : DARKGRAY;

    //selected thakle >.........<
    const char* startText = (gs->menu_selection == 0) ? "> START GAME <" : "  START GAME  ";
    int startWidth = MeasureText(startText, 60);
    DrawTextEx(gs->cfonts.menu_font2,startText,(Vector2){ (s_width / 2) - (startWidth / 2), s_height / 2}, 60,0,startColor);

    //exit optn  >.......<
    const char* exitText = (gs->menu_selection == 1) ? "> EXIT <" : "  EXIT  ";
    int exitWidth = MeasureText(exitText, 60);
    DrawTextEx(gs->cfonts.menu_font2,exitText, (Vector2){(gs->menu_selection == 1) ? (s_width / 2) - (exitWidth / 2)-30 :(s_width / 2) - (exitWidth / 2), s_height / 2 + 80}, 60, 0,exitColor);
}





void updateNameEntry(GS* gs) {
    //name input nibe
    int key = GetCharPressed();
    while (key > 0) {
        // only A-Z, a-z ,0-9 ,24 er letter er besi noy 
        if ((key >= 32) && (key <= 125) && (gs->nameLetterCount < 24)) {
            gs->playerName[gs->nameLetterCount] = (char)key;
            gs->playerName[gs->nameLetterCount + 1] = '\0';
            gs->nameLetterCount++;
        }
        key = GetCharPressed();
    }

    // Backspace chaple okkhor muche  jabe 
    if (IsKeyPressed(KEY_BACKSPACE)) {
        gs->nameLetterCount--;
        if (gs->nameLetterCount < 0) gs->nameLetterCount = 0;
        gs->playerName[gs->nameLetterCount] = '\0';
    }

    // ENTER chaple game start at least 1 ta letter likhtei hobe 
    if (IsKeyPressed(KEY_ENTER) && gs->nameLetterCount > 0) {
        gs->currentscreen = GAME;
    }
    updateParallax(gs,5.0f);
}

void drawNameEntry(GS* gs) {
    drawBackgroundMenu(gs);
    DrawRectangle(0,0,s_width,s_height,GetColor(0x000000AA));

    // rounded ekta box majhe 
    float boxWidth = 600.0f;
    float boxHeight = 300.0f;
    float boxX = (s_width / 2) - (boxWidth / 2);
    float boxY = (s_height / 2) - (boxHeight / 2);
    
    // DrawRectangleRounded((Rectangle){boxX, boxY, boxWidth, boxHeight}, 0.1f, 10, Fade(DARKGRAY, 0.9f));
    // DrawRectangleRoundedLines((Rectangle){boxX, boxY, boxWidth, boxHeight}, 0.1f, 10, GOLD);

    // title text 
    const char* title = "ENTER YOUR HERO NAME";
    int titleWidth = MeasureText(title, 30);
    DrawTextEx(gs->cfonts.menu_font2,title, (Vector2){(s_width / 2) - (titleWidth / 2), boxY + 40},0, 30, GOLD);

    // white color er name input deyar box
    DrawRectangle(boxX + 50, boxY + 120, boxWidth - 100, 60, Fade(LIGHTGRAY,.6f));
    DrawRectangleLines(boxX + 50, boxY + 120, boxWidth - 100, 60, Fade(LIGHTGRAY,.6f));

    // type kora player name 
    DrawTextEx(gs->cfonts.menu_font2,gs->playerName, (Vector2){boxX + 70, boxY + 135,}, 40,0, BLACK);

    // cursor blink 
    if ((int)(GetTime() * 3) % 2 == 0 && gs->nameLetterCount < 24) {
        int textW = MeasureText(gs->playerName, 40);
        DrawText(" _", boxX + 75 + textW, boxY + 135, 40, BLACK);
    }

    // instruction text 
    const char* instruction = "Press ENTER to Begin";
    int instWidth = MeasureText(instruction, 20);
    DrawTextEx(gs->cfonts.menu_font2,instruction, (Vector2){(s_width / 2) - (instWidth / 2), boxY + 230}, 20, 0,LIGHTGRAY);
}





//game over functions
void isGameover(GS* gs,float dt){
    if(gs->player.isDead && gs->currentscreen!=GAMEOVER && gs->player_animations[gs->current_player_anim_name].isfinished){
        gs->timer+=dt;
        if(gs->timer>=1.0f) gs->currentscreen = GAMEOVER;
    }
}

void drawGameover(GS* gs){
    drawBackgroundMenu(gs);
    DrawRectangle(0,0,s_width,s_height,Fade(BLACK,.6f));
    anim* a = &gs->player_animations[player_die];
    DrawTexturePro(
        a->tex,
        (Rectangle){
            .x = 3*a->frameWidth,
            .y = 0,
            .width = a->frameWidth,
            .height = a->frameHeight
        },
        (Rectangle){
            .x = s_width/2.0f-a->frameWidth/2.0f-160,
            .y = s_height/2.0f-a->frameHeight-380,
            .height = a->frameHeight*SPRITE_SCALE*1.8f,
            .width = a->frameWidth*SPRITE_SCALE*1.8f
        },
        (Vector2){0,0},
        0,WHITE
    );
    const char* gameover = "GAME OVER";
    int text_width = MeasureText(gameover,80);
    DrawTextEx(gs->cfonts.menu_font2,gameover,(Vector2){s_width/2.0f-text_width/2.0f,s_height/2.0f},80,0,RED);

}
