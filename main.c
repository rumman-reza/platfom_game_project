#include"raylib.h"
#include<stdbool.h>
#include<math.h>

// add enemy hurt animation and fix enemy damage code and find out why it's not working
#include"types.h"
#include"game.h"
#include"enemy.h"
#include"texture.h"
#include"animation.h"
#include "health.h"
int main(){
    
    InitWindow(1920,1080,"Epic Adventure");
    SetTargetFPS(60);
    // structure gulo define kora and 0 diye initialize kora
    GS gs={0};
    tex tex={0};
    anim anim = {0};
    // game er shob kichu initialize kora 
    initGame(&gs,&tex,&anim);

    while(!WindowShouldClose() && !gs.quit_game){
        
        float dt = GetFrameTime();
        updateGame(&gs,&anim,dt);
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (gs.currentscreen == MENU) {
            drawMenu(&gs); //menu er defination ache game.c te prototype game.h
        } 
        else if (gs.currentscreen == NAME_ENTRY) {
            // name screen draw hobe ekhane 
            drawNameEntry(&gs);
        }
        else if (gs.currentscreen == GAME) {
            BeginMode2D(gs.camera);
            drawGame(&gs);
            EndMode2D();
            drawHealthUI(&gs); 
        }else if(gs.currentscreen == GAMEOVER){
            drawGameover(&gs);
        }
        EndDrawing();
    }
    unloadTexture(&tex);
    unloadenemy(&gs);
    CloseWindow();
    
}