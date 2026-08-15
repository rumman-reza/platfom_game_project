#include"raylib.h"
#include "raymath.h"


#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))


#define s_height GetScreenHeight()
#define s_width GetScreenWidth()
#define pSpeed 1000.0f
#define pSpeedAir 600.0f
#define jumpSpeed 800.0f
#define gravity 1200.0f
#define MaxChunkNum 5

typedef enum gameScreen{
    MENU=1,GAME=0
} gamescreen;

typedef struct groundChunk
{
    Rectangle groundChunkRect;
}groundChunk;


typedef struct Player{
    float height;
    float width;

    Vector2 position;
    Vector2 velocity;
    Vector2 initial_position;
    bool isgrounded;
}Player;

typedef struct gameState
{
    gamescreen currentscreen;
    Player player;
    Camera2D camera;

    //ground stuff
    groundChunk gchunk[MaxChunkNum];
    float next_spawn_point;
    int chunk_index;

    // restricting left movement
    float clamp_left;

}GS;


void drawGame(GS* gs);
void initGame(GS* gs);
void updateGame(GS* gs, float dt);
void updateGameplay(GS* gs,float dt);
Rectangle getPlayerRect(GS* gs);
void playerMovement(GS* gs,float dt);
void Gravity(GS* gs,float dt);
void cameraMovement(GS* gs);
void checkBoundary(GS* gs);
void updateGround(GS *gs);


int main(){
    
    InitWindow(1920,1080,"practise");
    SetTargetFPS(60);
    
    GS gs={0};
    initGame(&gs);
    
    while(!WindowShouldClose()){
        
        float dt = GetFrameTime();
        updateGame(&gs,dt);
        
        BeginDrawing();
        
        ClearBackground(RAYWHITE);

        BeginMode2D(gs.camera);

        drawGame(&gs);
        
        EndMode2D();

        EndDrawing();
    }
    
    CloseWindow();
    
}

Rectangle drawPlayerRect(GS* gs){
    return (Rectangle){gs->player.position.x,gs->player.position.y,gs->player.width,gs->player.height};
}

void initGame(GS* gs){
    float ground_y = s_height*3.7f/4;
    float ground_height = s_height-ground_y;
    SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);

//set player
    gs->player.height=140;
    gs->player.width=80;
    gs->player.initial_position.x=s_width/2.0f;
    gs->player.initial_position.y=ground_y-gs->player.height;

    gs->player.position.x=s_width/2.0f;
    gs->player.position.y=ground_y-gs->player.height;
  
//set camera 
    gs->camera.offset = (Vector2){s_width/2.0f,s_height/2.0f};
    gs->camera.rotation = 0.0f;
    gs->camera.zoom = 1.0f;

    gs->camera.target = (Vector2){gs->player.position.x,gs->player.position.y};
    gs->clamp_left = 0.0f;
// setup initial Ground
    gs->next_spawn_point=0;
    gs->chunk_index=0;

    for(int i=0;i<MaxChunkNum;i++){
        gs->gchunk[i].groundChunkRect = (Rectangle){gs->next_spawn_point,ground_y,s_width,ground_height};
        gs->next_spawn_point+=s_width;
    }
}

void updateGame(GS* gs,float dt){
    switch(gs->currentscreen){
        case GAME: updateGameplay(gs,dt); break;
    }
}

void Gravity(GS* gs,float dt){
    gs->player.velocity.y += gravity*dt;
}

void groundedCheck(GS* gs,float dt){
    
    for(int i=0;i<MaxChunkNum;i++){
        if(CheckCollisionRecs(drawPlayerRect(gs),gs->gchunk[i].groundChunkRect)){
            gs->player.isgrounded=true;
            gs->player.position.y = gs->gchunk[0].groundChunkRect.y-gs->player.height;
            gs->player.velocity.y=0;
            break;
        }
    else gs->player.isgrounded=false;

    }
}

void updateGameplay(GS* gs,float dt){

    updateGround(gs);
    Gravity(gs,dt);
    playerMovement(gs,dt);
    checkBoundary(gs);
    groundedCheck(gs,dt);
    cameraMovement(gs);
}

void drawGame(GS* gs){

//drawing the ground rectangles;
    for(int i=0;i<MaxChunkNum;i++){
        DrawRectangleRec(gs->gchunk[i].groundChunkRect,GetColor(0x224248FF));
        DrawRectangleLinesEx(gs->gchunk[i].groundChunkRect,3,BLACK);

    }

    DrawRectangleRec(drawPlayerRect(gs),GetColor(0xFF9A00FF));
    DrawRectangleLinesEx(drawPlayerRect(gs),3,BLACK);
}

void playerMovement(GS* gs,float dt){

    //check button input
    if(IsKeyDown(KEY_D) && gs->player.isgrounded) gs->player.velocity.x = pSpeed;
    else if(IsKeyDown(KEY_D) && !gs->player.isgrounded) gs->player.velocity.x = pSpeedAir;
    
    else if(IsKeyDown(KEY_A)&& gs->player.isgrounded) gs->player.velocity.x = -pSpeed;
    else if(IsKeyDown(KEY_A) && !gs->player.isgrounded) gs->player.velocity.x = -pSpeedAir;

    else gs->player.velocity.x=0;


    if(IsKeyPressed(KEY_SPACE) && gs->player.isgrounded){
        gs->player.velocity.y = -jumpSpeed;
        gs->player.isgrounded = false; 
    }

    //update the player postion after taking input
    gs->player.position = (Vector2) Vector2Add(gs->player.position,Vector2Scale(gs->player.velocity,dt));

}

void cameraMovement(GS* gs){
    gs->camera.target = (Vector2){gs->player.position.x,gs->player.position.y};
}

void checkBoundary(GS* gs){
    if(gs->player.position.x < gs->clamp_left) gs->player.position.x=gs->clamp_left;
}

void updateGround(GS* gs){
    float view_distance = gs->player.position.x + s_width;
    while(gs->next_spawn_point<view_distance){
        gs->gchunk[gs->chunk_index].groundChunkRect.x = gs->next_spawn_point;
        gs->next_spawn_point += s_width;
        gs->chunk_index = (gs->chunk_index+1) % MaxChunkNum;
    }
}