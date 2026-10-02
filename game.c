    #include <math.h>
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
    #include"types.h"
    #include "score.h"
    #include "sound.h"
    #include "explosion.h"
    #include"tutorial.h"
    void drawGame(GS* gs,tex *textures){
    
    //drawing background elements

    drawBackground(gs);
    drawBushLine(gs, textures);
    drawDetailDecor(gs, textures);  
    DrawRectangle(gs->camera.target.x-s_width,0,2*s_width,s_height,GetColor(0x00000080));

    for(int i=0;i<MaxChunkNum;i++){

        float width = gs->gchunk[i].texture.width;
        float height = gs->gchunk[i].texture.height;
        Rectangle chunk = gs->gchunk[i].groundChunkRect;
        Rectangle source = (Rectangle){
            .height = height,
            .width = width,
            .x = 0,
            .y = 0
        };
        Rectangle dest = (Rectangle){
            .height = chunk.height,
            .width = chunk.width,
            .x = chunk.x,
            .y = chunk.y
        };
        
        DrawTexturePro(gs->gchunk[i].texture,source,dest,(Vector2){0,0},0,WHITE);

        
        // DrawRectangleLinesEx(gs->gchunk[i].groundChunkRect,3,BLACK);
        
        
        // ei chunk e heal item thakle  and seta pick na kore
        //thakle box ta green 
        if(gs->gchunk[i].hasHealthItem && !gs->gchunk[i].healthItemCollected){
            Rectangle source ={0,0,textures->health_item.width, textures->health_item.height};

         float floatOffset =  3.0*sinf(GetTime()* 6.0f)*8.0f;

           Rectangle dest = {
            .x = gs->gchunk[i].healthItemRect.x,
            .y = gs->gchunk[i].healthItemRect.y + floatOffset,
            .width = gs->gchunk[i].healthItemRect.width,
            .height = gs->gchunk[i].healthItemRect.height
        };
        
        DrawTexturePro(textures->health_item,source,dest,(Vector2){0,0},0.0f,WHITE);    
    }      
}    

    drawSpikes(gs);

    drawBombs(gs, textures);
    drawHealthDrops(gs, textures);
    drawExplosions(gs,textures);

    //drawing player sprite

        drawPlayerSprite(gs);
        // DrawRectangleLinesEx(getPlayerRect(gs),10,(gs->player.isattacking)?RED:BLUE);

    //drawing enemy sprites
        for(int i=0;i<max_enemy_num;i++){
           if(gs->enemy[i].isactive) drawEnemy(&gs->enemy[i]);
            // DrawRectangleLinesEx(getEnemyHitbox(&gs->enemy[i]),20,BLACK);
            // DrawRectangleLinesEx(getEnemyRect(&gs->enemy[i]),10,BLUE);
        }
        drawFog(gs);
        drawFogPuffs(gs,(float)GetTime());

        // drawPgasSprite(gs); 
        // drawPgasSprite(gs);
        
      for(int i=0;i<10;i++){
       if(gs->floatTexts[i].active){
        float alpha = gs->floatTexts[i].timer/  (gs->floatTexts[i].maxTime);

        Color textColor =Fade(gs->floatTexts[i].color,alpha);

       // DrawTextEx(gs->cfonts.menu_font1,gs->floatTexts[i].text,gs->floatTexts[i].position,30,0,textColor);
        
       DrawText(gs->floatTexts[i].text, (int)gs->floatTexts[i].position.x, (int)gs->floatTexts[i].position.y, 30, textColor);

       }
      }  
      if(gs->distance_traveled > 40000 && gs->distance_traveled < 41000){
        const char* hint = "!!!! DASH OVER THE GAPS !!!!";
        Vector2 size = MeasureTextEx(gs->cfonts.menu_font3, hint, 60, 0);

        float screenLeft = gs->camera.target.x - gs->camera.offset.x;   // left edge of the screen in world coords
        float x = screenLeft + s_width/2.0f - size.x/2.0f;

        float d = gs->distance_traveled;
        float alpha = 1.0f;
        if(d < 4300)      alpha = (d - 4000.0f) / 300.0f;   // fade in
        else if(d > 6700) alpha = (7000.0f - d) / 300.0f;   // fade out

        DrawTextEx(gs->cfonts.menu_font3, hint, (Vector2){x, 100}, 60, 0, Fade(LIGHTGRAY, alpha));
        }

    }

    void initGame(GS* gs,tex* tex,anim* anim){

        SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);

    // load textures
        loadTexture(tex,gs);

        gs->logo = LoadTexture("assets/PNG/Logo.png");
        
    // load animations
        loadAnimation(gs,tex);
   
       loadHighScores(gs->highScores);

        //menu 
        gs->currentscreen = MENU;
        gs->menu_selection = 0; //1st e start game e select hoye tahkbe 
        gs->quit_game = false;
        gs->show_tutorial = true;
        load_audio(gs);                      
        PlayMusicStream(gs->audio.menuMusic);
        gs->skip_duration = skip_timer;
        //set player
        float scale = SPRITE_SCALE * 1.6f;
        float frameW = gs->player_animations[player_idle].frameWidth;   // 80
        float frameH = gs->player_animations[player_idle].frameHeight;  // 48


        gs->player.width  = player_real_width  * scale;   // 17 * scale
        gs->player.height = player_real_height * scale;   // 32 * scale

        gs->player.collisionOffset.x = 30.0f * scale;
        gs->player.collisionOffset.y = 16.0f * scale;

        gs->player.initial_position.x = (frameW * scale) / 2.0f+200.0f;  
        gs->player.initial_position.y = ground_y - (frameH * scale);    
        
        gs->player.velocity.x = 300.0f;

        gs->player.position = gs->player.initial_position;


    //set camera 
        gs->camera.offset = (Vector2){s_width/2.0f-200.0f,0.0f};
        gs->camera.rotation = 0.0f;
        gs->camera.zoom = 1.0f;
        
        gs->camera.target = (Vector2){0.0f,0.0f};
        gs->last_camera_x = gs->camera.target.x;
        //heath function er variable gulo
        gs->player.maxHealth = PLAYER_MAX_HEALTH;
        gs->player.health = PLAYER_MAX_HEALTH;
        gs->player.isDead = false;
        
        // INITIAL GROUND / PATTERN GENERATION
        gs->chunk_index = 0;

        // Start spawning from the beginning of the world
        gs->next_spawn_point = -s_width;

        // No pattern has been spawned yet
        gs->lastPatternEndX = gs->next_spawn_point;

        // Distance before the first pattern
        gs->gapBetweenTheNextPattern = 2*s_width;

        // Generate the initial world
        updateGround(gs);
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
        // loading all the enemy information at the start of the game 
        for(int i=0;i<max_enemy_num;i++){
            gs->enemy[i] = loadEnemy(tex);
            gs->enemy[i].isactive = false; // spawn_pattern() activates slots as chunks generate
        }

        // setup poison gas cloud
        gs->pgas.position = (Vector2){-800.0f,ground_y-gs->pgas.pgas_anim[0].height+50.0f};
        gs->pgas.pgas_damage = 10.0f; 
        gs->pgas.frameduration = 0.08f;
        gs->pgas.attackcooldown = 2.0f;

        initFogPuffs(gs);   
        

        gs->starting_timer = STARTING_TIMER;

        // all other properties of gs are set to zero by default
        gs->last_bush_x = gs->next_spawn_point;


    }
    void unloadenemy(GS* gs){
        for(int i=0;i<max_enemy_num;i++){
            UnloadEnemyAnims(&gs->enemy[i]); 
        }
    }

    void updateGameplay(GS* gs,anim* anim,float dt){
        Rectangle pr = getPlayerRect(gs);
        gs->player.prevBottom = pr.y + pr.height;
        player_has_fallen(gs);
        playerDashUpdate(gs,dt);
        Gravity(gs,dt);
        hitting(gs,dt);
        playerMovement(gs,anim,dt);
        checkCeilingCollision(gs);
        checkWallCollision(gs); 
        restrict_left_movement(gs);
        groundedCheck(gs);
        setplayerstate(gs);
        updateJumpFrame(gs);
        DamageFromSpikes(gs,dt);
        DamageFromBombs(gs,dt);
        updateExplosions(gs, dt);
        updateAnimation(&gs->player_animations[gs->current_player_anim_name],dt);
        
        playerFootstepUpdate(gs, dt);

        updateHealth(gs,dt);
        checkHealthPickup(gs); // collidrawsion ditect check 

        updateGround(gs);
        cameraMovement(gs);
        updatescore(gs);
        move_pgas(gs,dt);
        updatePgasAnimation(gs,dt);


        float cameradelta = gs->camera.target.x - gs->last_camera_x;
        updateParallax(gs,cameradelta);
        gs->last_camera_x = gs->camera.target.x;


        //enemy functions
        updateEnemyInvultimer(gs,dt);
        updatePlayerInvulnerability(gs,dt);
        updateCombat(gs,dt);
        updateEnemy(gs,dt);
        updateEnemyAnimations(gs,dt);
        updateHealthDropPickup(gs); 
        for(int i = 0; i < 10; i++){
            if(gs->floatTexts[i].active){
                gs->floatTexts[i].position.y -= 40.0f * dt; 
                //40 px kore per sec ee uthbe 
            gs->floatTexts[i].timer -= dt;              
                
                if(gs->floatTexts[i].timer <= 0) {
                    gs->floatTexts[i].active = false;       
                }
            }
        }
        enemyFootstepUpdate(gs, dt);    
        isGameover(gs,dt);

    }

void restartGame(GS* gs) {
    // Player reset
    gs->player.health = PLAYER_MAX_HEALTH;
    gs->player.isDead = false;
    gs->player.position = gs->player.initial_position;
    gs->player.velocity = (Vector2){0, 0};
    gs->current_player_anim_name = player_idle;
    gs->player.isattacking = false;
    gs->player.isdashing = false;

//camera
    float player_center_x = gs->player.position.x + gs->player.collisionOffset.x + gs->player.width / 2.0f;
    gs->camera.target = (Vector2){player_center_x - camera_half_deadzone, 0.0f};
    gs->last_camera_x = gs->camera.target.x;

    // World & Chunks reset

    gs->chunk_index = 0;
    gs->next_spawn_point = -s_width;
    gs->lastPatternEndX = gs->next_spawn_point;
    gs->gapBetweenTheNextPattern = 2 * s_width;

    for(int i = 0; i < MaxChunkNum; i++){
        gs->gchunk[i].groundChunkRect = (Rectangle){0,0,0,0};
        gs->gchunk[i].hasHealthItem = false;
        gs->gchunk[i].healthItemCollected = false;
        gs->gchunk[i].healthItemRect = (Rectangle){0,0,0,0};
    }
    gs->spike_index = 0;
    gs->bomb_index = 0;
    gs->healthDrop_index = 0;
    gs->explosion_index = 0;
    //bomb

    for (int i = 0; i < max_bombs; i++) {
        gs->bombs[i].isactive = false;
    }
    for (int i = 0; i < max_spikes; i++) {
        gs->spikes[i].isactive = false;
    }
     for (int i = 0; i < max_enemy_num; i++) {
        gs->enemy[i].isactive = false;
    }
    //
    //
    // Poison gas reset
    gs->pgas.position = (Vector2){-200.0f, ground_y - gs->pgas.pgas_anim[0].height + 50.0f};

    gs->score = 0;
    gs->timer = 0.0f;
    
    gs->last_bush_x = gs->next_spawn_point;
    gs->bushDecor_index = 0;
    gs->detailDecor_index = 0;
    
    gs->starting_timer = STARTING_TIMER;   // otherwise the intro run only happens on the first game
    gs->current_player_state = idle_player;
    gs->player.invultimer = 0; gs->player.dashcooldowntimer = 0; gs->player.dashduration = 0;
    gs->player.hitduration = 0; gs->player.hashitthiswing = false;
    gs->spike_cooldown = 0; gs->pgas.attacktimer = 0;
    
    for(int i=0;i<max_bush_decor;i++)   gs->bushDecor[i].active = false;
    for(int i=0;i<max_detail_decor;i++) gs->detailDecor[i].active = false;
    for(int i=0;i<max_health_drops;i++) gs->healthDrops[i].active = false;
    for(int i=0;i<max_explosions;i++)   gs->explosions[i].active = false;
    for(int i=0;i<10;i++)               gs->floatTexts[i].active = false;
    gs->player.facing_left = false;
    gs->distance_traveled = 0;
    updateGround(gs);
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
        case TUTORIAL:
            updateTutorial(gs, dt);  
            break;
        case GAMEOVER:
            updateGameover(gs);
            break;
        case CREDITS:            
            updateCredits(gs);
            break;    
    }
}



//menu functions

void updateMenu(GS* gs) {
    HideCursor();
   //down key niche toggle korar jonno 
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        gs->menu_selection++;
        PlaySound(gs->audio.menu_select);
        if (gs->menu_selection > 3) gs->menu_selection = 0; // 3 tar besi option nai tao 0 te chole jabe 
    }
    //up key te vice versa
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        gs->menu_selection--;
        PlaySound(gs->audio.menu_select);
        if (gs->menu_selection < 0) gs->menu_selection = 3;
    }

    //enter key
    if (IsKeyPressed(KEY_ENTER)) {
        PlaySound(gs->audio.menu_click);
        if (gs->menu_selection == 0) {
            gs->currentscreen = NAME_ENTRY; // name page 
            gs->nameLetterCount = 0;        // name reset kora
            gs->playerName[0] = '\0';
        } 
        else  if(gs->menu_selection==1){
            gs->currentscreen=CREDITS;
        }
        else  if(gs->menu_selection==2){
            gs->pressed_how_to_play = true;
            gs->currentscreen=TUTORIAL;
        }

        else if (gs->menu_selection == 3) {
            gs->quit_game = true;
        }
        
    }
    updateParallax(gs,5.0f);
}

void drawMenu(GS* gs, tex* tex) {
    drawBackgroundMenu(gs);
    
    DrawRectangle(0,0,s_width,s_height,GetColor(0x000000AA));

    float width = tex->air_attack1.width/7.0f;
    float height = tex->air_attack1.height;
    float player_posx = s_width/2.0f-250.0f;
    float player_posy = s_height/2.0f+200.0f;
    DrawTexturePro(
        tex->air_attack1,(Rectangle){.x = 2*width,.y=0,.height = height, .width = width}, 
        (Rectangle){.x = player_posx+80.0f,.y = player_posy+50.0f,.width = width*SPRITE_SCALE*1.3f, .height = height*SPRITE_SCALE*1.3f},
        (Vector2){0,0},0,WHITE
    );

    float width2 = tex->enemy_hurt.width/8.0f;
    float height2  = tex->enemy_hurt.height;
    DrawTexturePro(
        tex->enemy_hurt,(Rectangle){.x = 2*width2,.y=0,.height = height2, .width = -width2}, 
        (Rectangle){.x = player_posx+475.0f,.y = player_posy+120.0f,.width = width2*SPRITE_SCALE*1.6f, .height = height2*SPRITE_SCALE*1.6f},
        (Vector2){0,0},0,WHITE
    );

    float width3 = tex->enemy_attack.width/18.0f;
    float height3  = tex->enemy_attack.height;
    DrawTexturePro(
        tex->enemy_attack,(Rectangle){.x = 5*width3,.y=0,.height = height3, .width = width3}, 
        (Rectangle){.x = player_posx-100.0f,.y = player_posy+90.0f,.width = width3*SPRITE_SCALE*1.6f, .height = height3*SPRITE_SCALE*1.6f},
        (Vector2){0,0},0,WHITE
    );

    float position = 0;
    float widthG = gs->gchunk[0].texture.width;
    float heightG = gs->gchunk[0].texture.height;
    while(position<=s_width){
        Rectangle source = (Rectangle){
            .height = heightG,
            .width = widthG,
            .x = 0,
            .y = 0
        };
        Rectangle dest = (Rectangle){
            .height = heightG*2.0f+40.0f,
            .width = widthG*4.0f,
            .x = position,
            .y = player_posy+270.0f
        };
        
        DrawTexturePro(gs->gchunk[0].texture,source,dest,(Vector2){0,0},0,WHITE);
        position+=widthG*4.0f;
    }
    DrawRectangle(0,0,s_width,s_height,GetColor(0x00000022));
    DrawRectangle(s_width/2.0f-gs->logo.width/2.0f,150.0f,gs->logo.width,gs->logo.height,GetColor(0x00000044));
    DrawTexture(gs->logo,s_width/2.0f-gs->logo.width/2.0f,150.0f,WHITE);

    // float widthB = tex->health_item.width;
    // float heightB = tex->health_item.height;
    // Rectangle source2 = (Rectangle){
    //     .height = heightB,
    //     .width = widthB,
    //     .x = 2*widthB,
    //     .y = 0
    // };
    // Rectangle dest2 = (Rectangle){
    //     .height = heightB/2.0f-30,
    //     .width = widthB/2.0f-30,
    //     .x = player_posx+320.0f,
    //     .y = player_posy+height*SPRITE_SCALE*1.8f-60
    // };
    
    // DrawTexturePro(tex->health_item,source2,dest2,(Vector2){0,0},0,WHITE);

    Color startColor   = (gs->menu_selection == 0) ? WHITE : DARKGRAY;
    Color creditsColor = (gs->menu_selection == 1) ? WHITE : DARKGRAY;
    Color tutorial_color = (gs->menu_selection == 2)? WHITE:DARKGRAY;
    Color exitColor    = (gs->menu_selection == 3) ? WHITE : DARKGRAY;

    const char* startText = (gs->menu_selection == 0) ? "> START GAME <" : "  START GAME  ";
    DrawTextEx(gs->cfonts.menu_font2, startText, (Vector2){700.0f, s_height / 2 - 50.0f}, 60, 0, startColor);

    const char* creditsText = (gs->menu_selection == 1) ? "> CREDITS <" : "  CREDITS  ";
    DrawTextEx(gs->cfonts.menu_font2, creditsText, (Vector2){760.0f, s_height / 2 + 25.0f}, 60, 0, creditsColor);

    const char* tutorialText = (gs->menu_selection == 2) ? "> TUTORIAL <" : "  TUTORIAL ";
    DrawTextEx(gs->cfonts.menu_font2, tutorialText, (Vector2){760.0f, s_height / 2 + 100.0f}, 60, 0, tutorial_color);

    const char* exitText = (gs->menu_selection == 3) ? "> EXIT <" : "  EXIT  ";
    DrawTextEx(gs->cfonts.menu_font2, exitText, (Vector2){820.0f, s_height / 2 + 175.0f}, 60, 0, exitColor);
}




void updateNameEntry(GS* gs) {
    //name input nibe
    int key = GetCharPressed();
    while (key > 0) {
        // only A-Z, a-z ,0-9 ,24 er letter er besi noy 
        if ((key >= 32) && (key <= 125) && (gs->nameLetterCount < 24)) {
            PlaySound(gs->audio.typing);
            gs->playerName[gs->nameLetterCount] = (char)key;
            gs->playerName[gs->nameLetterCount + 1] = '\0';
            gs->nameLetterCount++;
        }
        key = GetCharPressed();
    }

    // Backspace chaple okkhor muche  jabe 
    if (IsKeyPressed(KEY_BACKSPACE)) {
        PlaySound(gs->audio.menu_select);
        gs->nameLetterCount--;
        if (gs->nameLetterCount < 0) gs->nameLetterCount = 0;
        gs->playerName[gs->nameLetterCount] = '\0';
    }

    // ENTER chaple game start at least 1 ta letter likhtei hobe 
    if (IsKeyPressed(KEY_ENTER) && gs->nameLetterCount > 0) {
        PlaySound(gs->audio.menu_click); 
        restartGame(gs);
        StopMusicStream(gs->audio.menuMusic);
        PlayMusicStream(gs->audio.gameMusic);
        gs->currentscreen = gs->show_tutorial ? TUTORIAL : GAME;
        EnableCursor();
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
    
    Texture2D tex = gs->player_animations[player_idle].tex;
    float player_posx = boxX + boxWidth/2.0f - tex.width/2.0f-150.0f;
    float player_posy = boxY-tex.height-80.0f;
    DrawTexturePro(
        tex,(Rectangle){.x = 0,.y=0,.height = tex.height, .width = tex.width}, 
        (Rectangle){.x = player_posx,.y = player_posy,.width = tex.width*SPRITE_SCALE*1.6f, .height = tex.height*SPRITE_SCALE*1.6f},
        (Vector2){0,0},0,WHITE
    );
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
    int width = MeasureText(gs->playerName,40.0f);

    DrawTextEx(gs->cfonts.menu_font3,gs->playerName, (Vector2){boxX + 70, boxY + 135,}, 40,0, BLACK);

    DrawTextEx(gs->cfonts.menu_font3,gs->playerName, (Vector2){player_posx+tex.width/2.0f-width/2.0f+160.0f, player_posy,}, 40,0, YELLOW);


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
void isGameover(GS* gs, float dt){
    if(gs->player.isDead && gs->currentscreen != GAMEOVER && gs->player_animations[gs->current_player_anim_name].isfinished){
        gs->timer += dt;
        if(gs->timer >= 1.0f) {
            StopMusicStream(gs->audio.gameMusic);
            PlayMusicStream(gs->audio.menuMusic);
            gs->currentscreen = GAMEOVER; 
            gs->isNewHighScore = tryAddHighScore(gs->highScores, gs->playerName, gs->score);
            PlaySound(gs->audio.gameOverSting);
        }
    }
}

// void drawGameover(GS* gs){
//     drawBackgroundMenu(gs);
//     DrawRectangle(0,0,s_width,s_height,Fade(BLACK,.6f));
//     anim* a = &gs->player_animations[player_die];
//     DrawTexturePro(
//         a->tex,
//         (Rectangle){
//             .x = 3*a->frameWidth,
//             .y = 0,
//             .width = a->frameWidth,
//             .height = a->frameHeight
//         },
//         (Rectangle){
//             .x = s_width/2.0f-a->frameWidth/2.0f-160,
//             .y = s_height/2.0f-a->frameHeight-380,
//             .height = a->frameHeight*SPRITE_SCALE*1.8f,
//             .width = a->frameWidth*SPRITE_SCALE*1.8f
//         },
//         (Vector2){0,0},
//         0,WHITE
//     );
//     const char* gameover = "GAME OVER";
//     int text_width = MeasureText(gameover,80);
//     DrawTextEx(gs->cfonts.menu_font2,gameover,(Vector2){s_width/2.0f-text_width/2.0f,s_height/2.0f},80,0,RED);

//     drawGameOverScores(gs,gs->isNewHighScore);
// }

void player_has_fallen(GS* gs){
    if(getPlayerRect(gs).y>=ground_y+gs->player.height/2.0f){
        PlaySound(gs->audio.die);
        gs->player.isDead = true;
    }
}

void updatescore(GS* gs){
    gs->distance_traveled = gs->player.position.x -gs->player.initial_position.x;
    gs->score = gs->distance_traveled*SCORE_PER_DISTANCE;
}

void drawScoreHUD(const GS* gs) {
    const char* scoreText = TextFormat("SCORE: %06d", gs->score);

    Vector2 textSize = MeasureTextEx(gs->cfonts.menu_font2, scoreText, 40, 0);
    
    float scoreX = s_width/2.0f-textSize.x/2.0f+200.0f; 
    float scoreY = 30.0f; 


    DrawTextEx(gs->cfonts.menu_font3, scoreText, (Vector2){scoreX + 3, scoreY + 3}, 40, 0, Fade(BLACK, 0.6f));
    DrawTextEx(gs->cfonts.menu_font3, scoreText, (Vector2){scoreX, scoreY}, 60, 0, WHITE);
}



void updateGameover(GS* gs){
    
    if (IsKeyPressed(KEY_ENTER)) {
        gs->currentscreen = MENU;
        gs->menu_selection = 0; 
        StopMusicStream(gs->audio.gameMusic);
        PlayMusicStream(gs->audio.menuMusic);
    }
    updateParallax(gs, 1.5f);
}


void drawGameover(GS* gs){
    drawBackgroundMenu(gs);
    DrawRectangle(0,0,s_width,s_height,Fade(BLACK,.8f)); 
    
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
    Vector2 goSize = MeasureTextEx(gs->cfonts.menu_font3, gameover, 80, 0);
    DrawTextEx(gs->cfonts.menu_font3, gameover, (Vector2){(s_width/2.0f) - (goSize.x/2.0f), 80}, 80, 0, RED);

    drawGameOverScores(gs, gs->isNewHighScore);

  const char* instruction = "Press ENTER to return to Menu";
    Vector2 instSize = MeasureTextEx(gs->cfonts.menu_font3, instruction, 30, 0);
    
  
    if ((int)(GetTime() * 2) % 2 == 0) {
        DrawTextEx(gs->cfonts.menu_font3, instruction, (Vector2){(s_width/2.0f) - (instSize.x/2.0f), s_height - 100}, 30, 0, LIGHTGRAY);
    }
}



void updateCredits(GS* gs) {
    // esc ba enter chaple abar menu te ferot jabe
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_BACKSPACE)) {
        gs->currentscreen = MENU;
        gs->menu_selection = 0;
    }
    updateParallax(gs, 5.0f); // background scroll korar jonno
}

void drawCredits(GS* gs, tex* textures) {
    drawBackgroundMenu(gs);
    DrawRectangle(0, 0, s_width, s_height, Fade(BLACK, 0.85f));

    float centerX = s_width / 2.0f;

    const char* title = "DEVELOPED BY";
    Vector2 titleSize = MeasureTextEx(gs->cfonts.menu_font1, title, 90, 0);
    DrawTextEx(gs->cfonts.menu_font1, title, (Vector2){centerX - (titleSize.x / 2.0f), 200}, 90, 0, RAYWHITE);

    float leftCenter = centerX - 350.0f;
    float rightCenter = centerX + 350.0f;
    
    DrawTextEx(gs->cfonts.menu_font3, "SUPERVISOR - Al Muhit Muhtadi Sir", (Vector2){centerX - (titleSize.x / 2.0f)-100, 60}, 60, 0, RAYWHITE);
    
    float photoY = 380.0f; 
    float photoSize = 280.0f;

    if (textures->rumman_photo.id != 0) {
        Rectangle rummanSource = { 0.0f, 0.0f, (float)textures->rumman_photo.width, (float)textures->rumman_photo.height };
        Rectangle rummanDest = { leftCenter - (photoSize / 2.0f), photoY, photoSize, photoSize };
        DrawTexturePro(textures->rumman_photo, rummanSource, rummanDest, (Vector2){0,0}, 0.0f, WHITE);
    }
    Rectangle rummanBox = { leftCenter - (photoSize / 2.0f), photoY, photoSize, photoSize };
    DrawRectangleLinesEx(rummanBox, 4.0f, LIGHTGRAY);

    const char* name1 = "AHMED REZA RUMMAN";
    Vector2 n1Size = MeasureTextEx(gs->cfonts.menu_font2, name1, 35, 0);
    DrawTextEx(gs->cfonts.menu_font2, name1, (Vector2){leftCenter - (n1Size.x / 2.0f), photoY + photoSize + 20}, 35, 0, RAYWHITE);
    
    DrawText("ROLL: 2505011", leftCenter - 75, photoY + photoSize + 65, 22, LIGHTGRAY);

    if (textures->sifat_photo.id != 0) {
        Rectangle sifatSource = { 0.0f, 0.0f, (float)textures->sifat_photo.width, (float)textures->sifat_photo.height };
        Rectangle sifatDest = { rightCenter - (photoSize / 2.0f), photoY, photoSize, photoSize };
        DrawTexturePro(textures->sifat_photo, sifatSource, sifatDest, (Vector2){0,0}, 0.0f, WHITE);
    }
    Rectangle sifatBox = { rightCenter - (photoSize / 2.0f), photoY, photoSize, photoSize };
    DrawRectangleLinesEx(sifatBox, 4.0f, LIGHTGRAY);

    const char* name2 = "MD. SHAHRIAR SIFAT";
    Vector2 n2Size = MeasureTextEx(gs->cfonts.menu_font2, name2, 35, 0);
    DrawTextEx(gs->cfonts.menu_font2, name2, (Vector2){rightCenter - (n2Size.x / 2.0f), photoY + photoSize + 20}, 35, 0, RAYWHITE);
    
    DrawText("ROLL: 2505026", rightCenter - 75, photoY + photoSize + 65, 22, LIGHTGRAY);

    float bottomY = s_height - 180.0f;
    
    const char* soundCredit = "Sound Credit : pixabay";
    Vector2 scSize = MeasureTextEx(gs->cfonts.menu_font2, soundCredit, 28, 0);
    DrawTextEx(gs->cfonts.menu_font2, soundCredit, (Vector2){centerX - (scSize.x / 2.0f), bottomY}, 28, 0, LIGHTGRAY);

    const char* spriteCredit = "Sprite Credit : itch.io";
    Vector2 spSize = MeasureTextEx(gs->cfonts.menu_font2, spriteCredit, 28, 0);
    DrawTextEx(gs->cfonts.menu_font2, spriteCredit, (Vector2){centerX - (spSize.x / 2.0f), bottomY + 35}, 28, 0, LIGHTGRAY);

    const char* back = "Press ESC or ENTER to return";
    Vector2 backSize = MeasureTextEx(gs->cfonts.menu_font2, back, 22, 0);
    DrawTextEx(gs->cfonts.menu_font2, back, (Vector2){centerX - (backSize.x / 2.0f), s_height - 50}, 22, 0, DARKGRAY);
}