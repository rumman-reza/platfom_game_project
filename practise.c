#include "raylib.h"

typedef struct groundChunk
{
    Rectangle groundChunkRect;
}groundChunk;


typedef enum gameScreen{
    MENU=1,GAME=0
} gamescreen;

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


void initgame(GS* gs);
void updategameplay(GS* gs,float dt);
void drawGame(GS* gs);
void unloadGame(GS* gs);

int main(){


    GS gs = {0};

    initgame(&gs);

    while(!WindowShouldClose()){

        float dt = GetFrameTime();
        updategameplay(&gs,dt);

        BeginDrawing();

        drawGame(&gs);

        EndDrawing();

    }

    


}


void initgame(GS* gs){
    gs->player.height=120;
}

void updategameplay(GS* gs,float dt){
    //logic
}

void drawGame(GS* gs){
    //draw
}