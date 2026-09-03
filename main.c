#include"raylib.h"
#include<stdbool.h>
#include<math.h>


#include"types.h"
#include"game.h"
#include"enemy.h"
#include"texture.h"
#include"animation.h"
#include "health.h"
int main(){
    
    InitWindow(1920,900,"practise");
    SetTargetFPS(60);
    ToggleBorderlessWindowed();
    // structure gulo define kora and 0 diye initialize kora
    GS gs={0};
    tex tex={0};
    anim anim = {0};
    // game er shob kichu initialize kora 
    initGame(&gs,&tex,&anim);
    Enemy testenemy = loadEnemy(&tex);
    
    while(!WindowShouldClose()){
        
        float dt = GetFrameTime();
        updateGame(&gs,&anim,dt);
        updateEnemy(&testenemy,&gs,dt);
        BeginDrawing();
        
        ClearBackground(RAYWHITE);

        BeginMode2D(gs.camera);

        drawGame(&gs);
        drawEnemy(&testenemy);
        EndMode2D();
           drawHealthUI(&gs);
        EndDrawing();
    }
    unloadTexture(&tex);
    UnloadEnemyAnims(&testenemy); 
    CloseWindow();
    
}