#include"raylib.h"
#include "raymath.h"
#include<stdbool.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))


#define s_height GetScreenHeight()
#define s_width GetScreenWidth()
#define pSpeed 1000.0f
#define pSpeedAir 600.0f
#define jumpSpeed 500.0f
#define gravity 1200.0f
#define MaxChunkNum 5
#define SPRITE_SCALE 3.0f

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
    bool facing_left;
    Vector2 position;
    Vector2 velocity;
    Vector2 initial_position;
    bool isgrounded;
}Player;


typedef struct texture{
//background elements
    Texture2D Background;
    Texture2D Woods_first;
    Texture2D Woods_second;
    Texture2D woods_third;
    Texture2D Woods_fourth;

// player textures    
    Texture2D idle;
    Texture2D running;
    Texture2D dash;
    Texture2D jump;
    Texture2D die;
}tex;

typedef struct animation{
    Texture2D tex;
    int frameWidth;
    int frameHeight;
    int framecount;
    int currentframe;
    float frameduration;
    float frametimer;

} anim;

typedef enum animations{
     player_idle,
     player_running,
     player_jump,
     player_dash,
     player_die,
     player_animations_number,
     background1,
     background2,
     background3,
     background4,
     background5

} anim_name;

typedef struct background{
    Texture2D tex[5];
    Rectangle source;
    Rectangle dest;
} bglayer;

typedef struct gameState
{
    gamescreen currentscreen;

    Player player;
    anim player_animations[player_animations_number];
    anim_name current_player_anim_name;


    Camera2D camera;
    //ground stuff
    groundChunk gchunk[MaxChunkNum];
    float next_spawn_point;
    int chunk_index;

    //background element
    bglayer bg[MaxChunkNum];
    float background_spawnpoint;
    int background_index;
    // restricting left movement
    float clamp_left;

}GS;


void drawGame(GS* gs);
void initGame(GS* gs, tex* tex, anim* anim);
void updateGame(GS* gs,anim* anim, float dt);
void updateGameplay(GS* gs,anim* anim,float dt);
Rectangle getPlayerRect(GS* gs);
void playerMovement(GS* gs,anim* anim,float dt);
void Gravity(GS* gs,float dt);
void cameraMovement(GS* gs);
void checkBoundary(GS* gs);
void updateGround(GS *gs);
void loadTexture(tex* tex, GS* gs);
void loadAnimation(GS* gs,tex* tex, anim* anim);
void updateAnimation(GS* gs,float dt);
void drawPlayerSprite(GS* gs);
void setAnimation(GS* gs,anim_name name);
Texture2D LoadPixelTexture(const char *path);
void drawBackground(GS* gs);
void unloadTexture(tex* tex);
void updateBackground(GS* gs);


int main(){
    
    InitWindow(1920,1080,"practise");
    SetTargetFPS(60);
    
    GS gs={0};
    tex tex={0};
    anim anim = {0};
    initGame(&gs,&tex,&anim);
    
    while(!WindowShouldClose()){
        
        float dt = GetFrameTime();
        updateGame(&gs,&anim,dt);
        
        BeginDrawing();
        
        ClearBackground(RAYWHITE);

        BeginMode2D(gs.camera);

        drawGame(&gs);
        
        EndMode2D();

        EndDrawing();
    }
    unloadTexture(&tex);
    CloseWindow();
    
}

Rectangle drawPlayerRect(GS* gs){
    return (Rectangle){gs->player.position.x,gs->player.position.y,gs->player.width,gs->player.height};
}

void initGame(GS* gs,tex* tex,anim* anim){
    
    float ground_y = s_height*3.7f/4;
    float ground_height = s_height-ground_y;
    SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);

// load textures
    loadTexture(tex,gs);
// load animations
    loadAnimation(gs,tex,anim);

//set player
    gs->player.height = gs->player_animations[player_idle].frameHeight * SPRITE_SCALE;
    gs->player.width  = gs->player_animations[player_idle].frameWidth  * SPRITE_SCALE;

    gs->player.initial_position.x=s_width/2.0f;
    gs->player.initial_position.y=ground_y-gs->player.height;

    gs->player.position.x=s_width/2.0f;
    gs->player.position.y=ground_y-gs->player.height;
  
//set camera 
    gs->camera.offset = (Vector2){s_width/2.0f,0.0f};
    gs->camera.rotation = 0.0f;
    gs->camera.zoom = 1.0f;

    gs->camera.target = (Vector2){gs->player.position.x,0.0f};
    gs->clamp_left = 0.0f;
// setup initial Ground
    gs->next_spawn_point=-s_width;
    gs->chunk_index=0;

    for(int i=0;i<MaxChunkNum;i++){
        gs->gchunk[i].groundChunkRect = (Rectangle){gs->next_spawn_point,ground_y,s_width,ground_height};
        gs->next_spawn_point+=s_width;
    }
// setup background

    gs->background_spawnpoint=-s_width;
    gs->chunk_index=0;
    for(int i=0;i<MaxChunkNum;i++){
        gs->bg[i].source=(Rectangle){
            .x = 0,
            .y=0,
            .height=gs->bg[0].tex[0].height,
            .width=gs->bg[0].tex[0].width
        };
        gs->bg[i].dest=(Rectangle){
            .x=gs->background_spawnpoint,
            .y=-gs->gchunk[0].groundChunkRect.height,
            .height=gs->bg[0].tex[0].height*SPRITE_SCALE*1.5f,
            .width=gs->bg[0].tex[0].width*SPRITE_SCALE*1.5f
        };
        gs->background_spawnpoint+=gs->bg[0].tex[0].width*SPRITE_SCALE*1.5f;
    }

}

void updateGame(GS* gs,anim* anim,float dt){
    switch(gs->currentscreen){
        case GAME: updateGameplay(gs,anim,dt); break;
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

void updateGameplay(GS* gs,anim* anim,float dt){
    updateAnimation(gs,dt);
    updateGround(gs);
    updateBackground(gs);
    Gravity(gs,dt);
    playerMovement(gs,anim,dt);
    checkBoundary(gs);
    groundedCheck(gs,dt);
    cameraMovement(gs);
}

void drawGame(GS* gs){
//drawing background elements

//drawing background elements

    drawBackground(gs);

//drawing the ground rectangles;
    for(int i=0;i<MaxChunkNum;i++){
        DrawRectangleRec(gs->gchunk[i].groundChunkRect,GetColor(0x224248FF));
        DrawRectangleLinesEx(gs->gchunk[i].groundChunkRect,3,BLACK);

    }
//drawing player sprite
    drawPlayerSprite(gs);
}

void playerMovement(GS* gs,anim* anim,float dt){

    //check button input
    if(IsKeyDown(KEY_D) && gs->player.isgrounded){ 
        gs->player.velocity.x = pSpeed;
        gs->player.facing_left=false;
        setAnimation(gs,player_running);
    }
    else if(IsKeyDown(KEY_D) && !gs->player.isgrounded){
        gs->player.velocity.x = pSpeedAir;
        gs->player.facing_left=false;
    }
    
    else if(IsKeyDown(KEY_A)&& gs->player.isgrounded) {
        gs->player.velocity.x = -pSpeed;
        gs->player.facing_left=true;
        setAnimation(gs,player_running);
    }
    else if(IsKeyDown(KEY_A) && !gs->player.isgrounded){
        gs->player.velocity.x = -pSpeedAir;
        gs->player.facing_left=true;
    }

    else{ 
        gs->player.velocity.x=0;
        if(gs->player.isgrounded) setAnimation(gs,player_idle);
    }


    if(IsKeyPressed(KEY_SPACE) && gs->player.isgrounded){
        gs->player.velocity.y = -jumpSpeed;
        gs->player.isgrounded = false; 
    }

    //update the player postion after taking input
    gs->player.position = (Vector2) Vector2Add(gs->player.position,Vector2Scale(gs->player.velocity,dt));

}

void cameraMovement(GS* gs){
    gs->camera.target = (Vector2){gs->player.position.x,0.0f};
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

void loadTexture(tex* tex, GS* gs){
    tex->idle = LoadPixelTexture("assets/sprites/Idle.png");
    tex->running = LoadPixelTexture("assets/sprites/Run.png");
    tex->jump = LoadPixelTexture("assets/sprites/JumpAndFall.png");
    tex->dash = LoadPixelTexture("assets/sprites/Dash.png");
    tex->die = LoadPixelTexture("assets/sprites/Die.png");


    tex->Background = LoadPixelTexture("assets/background_elements/BACKGROUND.png");
    tex->Woods_first = LoadPixelTexture("assets/background_elements/WOODSFirst.png");
    tex->Woods_second = LoadPixelTexture("assets/background_elements/WOODSSecond.png");
    tex->woods_third = LoadPixelTexture("assets/background_elements/WOODSThird.png");
    tex->Woods_fourth = LoadPixelTexture("assets/background_elements/WOODSFourth.png");
    
    for(int i=0;i<MaxChunkNum;i++){
        gs->bg[i].tex[background1-background1] = tex->Background;
        gs->bg[i].tex[background5-background1] = tex->Woods_first; 
        gs->bg[i].tex[background4-background1] = tex->Woods_second;    
        gs->bg[i].tex[background3-background1] = tex->woods_third;
        gs->bg[i].tex[background2-background1] = tex->Woods_fourth;
    }
}

void loadAnimation(GS* gs,tex* tex,anim* anim){

    gs->player_animations[player_idle].tex = tex->idle;
    gs->player_animations[player_idle].framecount = 1;
    gs->player_animations[player_idle].frameduration = 0.1f;
    gs->player_animations[player_idle].frameHeight = gs->player_animations[player_idle].tex.height;
    gs->player_animations[player_idle].frameWidth = gs->player_animations[player_idle].tex.width/gs->player_animations[player_idle].framecount;

    gs->player_animations[player_running].tex = tex->running;
    gs->player_animations[player_running].framecount = 8;
    gs->player_animations[player_running].frameduration = 0.08f;
    gs->player_animations[player_running].frameHeight = gs->player_animations[player_running].tex.height;
    gs->player_animations[player_running].frameWidth = gs->player_animations[player_running].tex.width/gs->player_animations[player_running].framecount;

    gs->player_animations[player_jump].tex = tex->jump;
    gs->player_animations[player_dash].tex = tex->dash;
    gs->player_animations[player_die].tex = tex->die;


    gs->current_player_anim_name = player_idle;

}
void updateAnimation(GS* gs,float dt){
    gs->player_animations[gs->current_player_anim_name].frametimer += dt;

    while(gs->player_animations[gs->current_player_anim_name].frametimer>=gs->player_animations[gs->current_player_anim_name].frameduration){
        gs->player_animations[gs->current_player_anim_name].frametimer-=gs->player_animations[gs->current_player_anim_name].frameduration;
        gs->player_animations[gs->current_player_anim_name].currentframe = (gs->player_animations[gs->current_player_anim_name].currentframe + 1)%(gs->player_animations[gs->current_player_anim_name].framecount);
    }    
}

void drawPlayerSprite(GS* gs){
    anim *a = &gs->player_animations[gs->current_player_anim_name];

    Rectangle source = {
        .x = a->currentframe * a->frameWidth,
        .y = 0,
        .width  = (gs->player.facing_left) ? -a->frameWidth : a->frameWidth,
        .height = a->frameHeight
    };
    Rectangle dest = {
        .x = gs->player.position.x,
        .y = gs->player.position.y,
        .width  = a->frameWidth  * SPRITE_SCALE,
        .height = a->frameHeight * SPRITE_SCALE
    };
    DrawTexturePro(a->tex, source, dest, (Vector2){0,0}, 0.0f, WHITE);
}

void setAnimation(GS* gs,anim_name name){
    if(gs->current_player_anim_name != name){
        gs->current_player_anim_name = name;
        gs->player_animations[name].currentframe=0;
        gs->player_animations[name].frametimer=0;
    }
}

Texture2D LoadPixelTexture(const char *path) {
    Texture2D t = LoadTexture(path);
    SetTextureFilter(t, TEXTURE_FILTER_POINT);
    return t;
}

void updateBackground(GS* gs){
    float viewing_distance = gs->player.position.x + s_width;
    while(gs->background_spawnpoint<viewing_distance){
        gs->bg[gs->background_index].dest.x = gs->background_spawnpoint;
        gs->background_spawnpoint += gs->bg[0].tex[0].width*SPRITE_SCALE*1.5f;
        gs->background_index = (gs->background_index+1)%MaxChunkNum;
    }

}


void drawBackground(GS* gs){
    
    for(int j=0;j<MaxChunkNum;j++){
        for(int i=0;i<5;i++){
            DrawTexturePro(gs->bg[j].tex[i],gs->bg[j].source,gs->bg[j].dest,(Vector2){0,0},0.0f,WHITE);
        }
    }
}


void unloadTexture(tex* tex){
    UnloadTexture(tex->Background);
    UnloadTexture(tex->dash);
    UnloadTexture(tex->die);
    UnloadTexture(tex->idle);
    UnloadTexture(tex->jump);
    UnloadTexture(tex->running);
    UnloadTexture(tex->Woods_first);
    UnloadTexture(tex->Woods_fourth);
    UnloadTexture(tex->Woods_second);
    UnloadTexture(tex->woods_third);
}