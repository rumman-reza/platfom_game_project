#include"raylib.h"
#include<stdbool.h>
#include<math.h>
#include"types.h"
#include"game.h"
#include"enemy.h"
#include"texture.h"
#include"animation.h"
#include "health.h"
#include "sound.h"
int main(){
    
    InitWindow(1920,1080,"Epic Adventure");
    SetTargetFPS(60);
    // ToggleBorderlessWindowed();
    // structure gulo define kora and 0 diye initialize kora
    InitAudioDevice();
    GS gs={0};
    tex tex={0};
    anim anim = {0};
    // game er shob kichu initialize kora 
    initGame(&gs,&tex,&anim);

    while(!WindowShouldClose() && !gs.quit_game){
        
        float dt = GetFrameTime();
        
        updateGame(&gs,&anim,dt);
        updateMusic(&gs);
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
            drawGame(&gs, &tex);
            
            EndMode2D();
            drawHealthUI(&gs); 
            drawScoreHUD(&gs);
            }
        else if(gs.currentscreen == GAMEOVER){
            drawGameover(&gs);
        }
        EndDrawing();       
    }
    unloadTexture(&tex);
    unloadenemy(&gs);
    unloadAudio(&gs);
    CloseAudioDevice();
    CloseWindow();
}