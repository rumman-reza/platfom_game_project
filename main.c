#include"raylib.h"
#include "raymath.h"
#include<stdbool.h>
#include<math.h>


#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

//bg layers
#define BG_LAYER_COUNT 6
//screen er size newa
#define s_height GetScreenHeight()
#define s_width GetScreenWidth()
//different player speeds
#define pSpeed 1000.0f
#define pAttackMoveSpeed 500.0f
#define pSpeedAir 600.0f
#define jumpSpeed 500.0f
#define gravity 1200.0f
#define dash_speed 2000.0f // jore laaf dewar speed
//different timers
#define dash_duration .4f
#define dash_cooldowntimer .6f
#define attackduration 1.12f
#define airattackduration .56f
// texture choto boro korar jonno
#define SPRITE_SCALE 3.0f
#define BG_SCALE (SPRITE_SCALE * 1.3f)
// attack koto tuku gulo frame er moddhe hobe 
#define attackstartframe 3
#define attackendframe 6    
//koto gulo ground chunk dekhabe
#define MaxChunkNum 5
// health  maximum and koto kore kombe seta 
#define PLAYER_MAX_HEALTH 100.0f
#define HEALTH_DECAY_RATE 5.0f  


// menu ar game e switch korar jonno enum
typedef enum gameScreen{
    MENU=1,GAME=0
} gamescreen;

// ground er jonno struct
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
    // jump ar air e movement detect korar jonno
    bool isgrounded;

    bool isdashing;
    float dashduration;
    float dashcooldowntimer; // ekta dash dewar por koto tuku time por abar dash dite parbe

    bool isattacking;
    float hitduration;
    // health  variable gulo 
    float health;  
    float maxHealth;   //maximum health koto
    bool isDead;

}Player;

typedef struct Enemy{
    float width;
    float height;
    Vector2 position;
    Vector2 velocity;
    Vector2 inital_position;
    bool facing_left;
    float attack_duration;
    float attack_cooldown;
    bool isgrounded;

} Enemy;


typedef struct texture{ // game er sob gulo texture ekhane store kora hoy and jekhane texture dorkar hoy 
                        // shekhane amra ei sturcture theke access kori
//background elements
    Texture2D Background;
    Texture2D Woods_first;
    Texture2D Woods_second;
    Texture2D woods_third;
    Texture2D Woods_fourth;
    Texture2D Bush_background;
// player textures    
    Texture2D idle;
    Texture2D running;
    Texture2D dash;
    Texture2D jumpandfall;
    Texture2D die;
    Texture2D attack1;
    Texture2D air_attack1;
// enemy textures
    Texture2D enemy_idle;
    Texture2D enemy_run;
    Texture2D enemy_attack;
}tex;

typedef struct animation{
    Texture2D tex;
    int frameWidth;
    int frameHeight;
    int framecount;
    int currentframe;
    float frameduration;
    float frametimer;// koto shomoy dhore frame screen ache 
    float timedependent;
    bool looping;
} anim;

typedef enum animations{
     player_idle,
     player_running,
     player_jump,
     player_dash,
     player_die,
     attack,
     airattack,
     player_animations_number,
     background1,
     background2,
     background3,
     background4,
     background5,
     background_layer_number

} anim_name;


typedef enum enemy_animations{
    enemy_idle,
    enemy_running,
    enemy_attack,
    enemy_anim_num
} enemy_anim;

typedef struct parallax_layer{ // durer jinish aste cholbe ar kacher jinish druto cholbe
    Texture2D tex;
    float scrollfactor;   // kon layer koto druto cholbe 
    float offsetX;        // player koto tuku shamne agaiche 
    Rectangle source;
} parallax_layer;

typedef struct background{
    parallax_layer layer[5];
} bglayer;

typedef struct gameState // main struct of this game, ekhane shob rokom game er element ache 
{
    gamescreen currentscreen; // game menu te naki game er vitore ta bujhai

    Player player;
    anim player_animations[player_animations_number];
    anim_name current_player_anim_name;


    Camera2D camera;
    float last_camera_x;

    //ground stuff
    groundChunk gchunk[MaxChunkNum];
    float next_spawn_point;
    int chunk_index;

    //background element
    parallax_layer bgLayers[BG_LAYER_COUNT];

    // restricting left movement
    float clamp_left;
    //enemy things
    Enemy enemy;
    anim enemy_animations[enemy_anim_num];
    anim_name current_enemy_anim_name;

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
void updateJumpFrame(GS* gs);
void playerDashUpdate(GS* gs,float dt);
Rectangle gethitbox(GS* gs);
void hitting(GS* gs,float dt);
void updateParallax(GS* gs,float cameradeltax);
void updateHealth(GS* gs, float dt);


int main(){
    
    InitWindow(1920,1080,"practise");
    SetTargetFPS(120);
    // structure gulo define kora and 0 diye initialize kora
    GS gs={0};
    tex tex={0};
    anim anim = {0};

    // game er shob kichu initialize kora 
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
    gs->player.height = gs->player_animations[player_idle].frameHeight * SPRITE_SCALE*1.6f;
    gs->player.width  = gs->player_animations[player_idle].frameWidth  * SPRITE_SCALE*1.6f;

    gs->player.initial_position.x=s_width/2.0f;
    gs->player.initial_position.y=ground_y-gs->player.height;

    gs->player.position.x=s_width/2.0f;
    gs->player.position.y=ground_y-gs->player.height;
//set enemy
    gs->enemy.height = gs->enemy_animations[enemy_idle].frameHeight * SPRITE_SCALE*1.60f;
    gs->enemy.width = gs->enemy_animations[enemy_idle].frameWidth * SPRITE_SCALE * 1.60f;
    

//set camera 
    gs->camera.offset = (Vector2){s_width/2.0f,0.0f};
    gs->camera.rotation = 0.0f;
    gs->camera.zoom = 1.0f;

    gs->camera.target = (Vector2){gs->player.position.x,0.0f};
    gs->last_camera_x = gs->camera.target.x;
    gs->clamp_left = 0.0f;
// setup initial Ground
    gs->next_spawn_point=-s_width;
    gs->chunk_index=0;

//heath function er variable gulo
    gs->player.maxHealth = PLAYER_MAX_HEALTH;
    gs->player.health = PLAYER_MAX_HEALTH;
    gs->player.isDead = false;



    for(int i=0;i<MaxChunkNum;i++){
        gs->gchunk[i].groundChunkRect = (Rectangle){gs->next_spawn_point,ground_y,s_width,ground_height};
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
    playerDashUpdate(gs,dt);
    Gravity(gs,dt);
    updateHealth(gs,dt); //decays HP hobe proti each frame
    hitting(gs,dt);
    playerMovement(gs,anim,dt);
    updateJumpFrame(gs);
    checkBoundary(gs);
    updateGround(gs);
    groundedCheck(gs,dt);
    cameraMovement(gs);
    
    float cameradelta = gs->camera.target.x - gs->last_camera_x;
    updateParallax(gs,cameradelta);
    gs->last_camera_x = gs->camera.target.x;

}

void drawGame(GS* gs){

//drawing background elements

    drawBackground(gs);

//drawing the ground rectangles;
    for(int i=0;i<MaxChunkNum;i++){
        DrawRectangleRec(gs->gchunk[i].groundChunkRect,DARKBROWN);
        // DrawRectangleLinesEx(gs->gchunk[i].groundChunkRect,3,BLACK);

    }
//drawing player sprite
    drawPlayerSprite(gs);
}

void playerMovement(GS* gs,anim* anim,float dt){


    if(gs->player.isdashing){ 
        gs->player.position = (Vector2)Vector2Add(gs->player.position,Vector2Scale(gs->player.velocity,dt));
        return;
    }
    //check button input
    if(IsKeyDown(KEY_D) && gs->player.isgrounded){ 

        if(gs->player.isattacking) gs->player.velocity.x = pAttackMoveSpeed;
        else gs->player.velocity.x = pSpeed;

        gs->player.facing_left=false;

        if(!gs->player.isattacking) setAnimation(gs,player_running);
    }
    else if(IsKeyDown(KEY_D) && !gs->player.isgrounded){
        gs->player.velocity.x = pSpeedAir;
        gs->player.facing_left=false;
    }
    
    else if(IsKeyDown(KEY_A)&& gs->player.isgrounded ) {
        if(gs->player.isattacking) gs->player.velocity.x = -pAttackMoveSpeed;
        else gs->player.velocity.x = -pSpeed;
        gs->player.facing_left=true;
        if(!gs->player.isattacking) setAnimation(gs,player_running);
    }
    else if(IsKeyDown(KEY_A) && !gs->player.isgrounded){
        gs->player.velocity.x = -pSpeedAir;
        gs->player.facing_left=true;
    }

    else{ 
        gs->player.velocity.x=0;
        if(gs->player.isgrounded && !gs->player.isattacking) setAnimation(gs,player_idle);
        else if(!gs->player.isgrounded && !gs->player.isattacking)setAnimation(gs,player_jump);
    }


    if(IsKeyPressed(KEY_SPACE) && gs->player.isgrounded){
        gs->player.velocity.y = -jumpSpeed;
        gs->player.isgrounded = false; 
        setAnimation(gs,player_jump);
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

void updateJumpFrame(GS* gs){
    if(gs->player.isgrounded && gs->current_player_anim_name != player_jump) return;
    else {
        if(gs->player.velocity.y<0) gs->player_animations[player_jump].currentframe=0;
        else if(gs->player.velocity.y>250.0f) gs->player_animations[player_jump].currentframe=1;
    }
}

void loadTexture(tex* tex, GS* gs){
    tex->idle = LoadPixelTexture("assets/sprites/Idle.png");
    tex->running = LoadPixelTexture("assets/sprites/Run.png");
    tex->jumpandfall = LoadPixelTexture("assets/sprites/JumpAndFall.png");
    tex->dash = LoadPixelTexture("assets/sprites/Dash.png");
    tex->die = LoadPixelTexture("assets/sprites/Die.png");
    tex->attack1 = LoadPixelTexture("assets/sprites/GroundCombo3.png");
    tex->air_attack1 = LoadPixelTexture("assets/sprites/AirCombo2.png");

    tex->Background   = LoadPixelTexture("assets/background_elements/BACKGROUND.png");
    tex->Woods_first  = LoadPixelTexture("assets/background_elements/WOODSFi.png");
    tex->Woods_second = LoadPixelTexture("assets/background_elements/WOODSSe.png");
    tex->woods_third  = LoadPixelTexture("assets/background_elements/WOODSTh.png");
    tex->Woods_fourth = LoadPixelTexture("assets/background_elements/WOODSFo.png");
    tex->Bush_background = LoadPixelTexture("assets/background_elements/BUSH_BACKGROUND.png");

    gs->bgLayers[0].tex = tex->Background;
    gs->bgLayers[1].tex = tex->Woods_first;
    gs->bgLayers[2].tex = tex->Woods_second;
    gs->bgLayers[3].tex = tex->woods_third;
    gs->bgLayers[4].tex = tex->Woods_fourth;
    gs->bgLayers[5].tex = tex->Bush_background;

    tex->enemy_idle = LoadPixelTexture("assets/enemy_sprites/SkeletonIdle.png");
    tex->enemy_run = LoadPixelTexture("assets/enemy_sprites/SkeletonWalk.png");
    tex->enemy_attack = LoadPixelTexture("assets/enemy_sprites/SkeletonAttack.png");




}


void loadAnimation(GS* gs,tex* tex,anim* animt){

    gs->player_animations[player_idle].tex = tex->idle;
    gs->player_animations[player_idle].framecount = 1;
    gs->player_animations[player_idle].frameduration = 0.1f;
    gs->player_animations[player_idle].frameHeight = gs->player_animations[player_idle].tex.height;
    gs->player_animations[player_idle].frameWidth = gs->player_animations[player_idle].tex.width/gs->player_animations[player_idle].framecount;
    gs->player_animations[player_idle].timedependent = true;
    gs->player_animations[player_idle].looping = true;

    gs->player_animations[player_running].tex = tex->running;
    gs->player_animations[player_running].framecount = 8;
    gs->player_animations[player_running].frameduration = 0.08f;
    gs->player_animations[player_running].frameHeight = gs->player_animations[player_running].tex.height;
    gs->player_animations[player_running].frameWidth = gs->player_animations[player_running].tex.width/gs->player_animations[player_running].framecount;
    gs->player_animations[player_running].timedependent = true;
    gs->player_animations[player_running].looping = true;



    gs->player_animations[player_jump].tex = tex->jumpandfall;
    gs->player_animations[player_jump].framecount=2;
    gs->player_animations[player_jump].timedependent=false;
    gs->player_animations[player_jump].frameHeight = gs->player_animations[player_jump].tex.height;
    gs->player_animations[player_jump].frameWidth = gs->player_animations[player_jump].tex.width/gs->player_animations[player_jump].framecount;
    gs->player_animations[player_jump].looping = false;

    gs->player_animations[player_dash].tex = tex->dash;
    gs->player_animations[player_dash].framecount = 5;
    gs->player_animations[player_dash].frameduration = 0.08f;
    gs->player_animations[player_dash].timedependent=true;
    gs->player_animations[player_dash].frameHeight=  gs->player_animations[player_dash].tex.height;
    gs->player_animations[player_dash].frameWidth=  gs->player_animations[player_dash].tex.width/6;
    gs->player_animations[player_dash].looping = false;


    anim* attack_ani = &gs->player_animations[attack];
    attack_ani->tex = tex->attack1;
    attack_ani->framecount = 14;
    attack_ani->frameduration = .08f;
    attack_ani->frameHeight = attack_ani->tex.height;
    attack_ani->frameWidth = attack_ani->tex.width/attack_ani->framecount;
    attack_ani->looping = true;
    attack_ani->timedependent = true;

    anim* air_attack_ani = &gs->player_animations[airattack];
    air_attack_ani->tex = tex->air_attack1;
    air_attack_ani->framecount = 7;
    air_attack_ani->frameduration = .08f;
    air_attack_ani->frameHeight = air_attack_ani->tex.height;
    air_attack_ani->frameWidth = air_attack_ani->tex.width/air_attack_ani->framecount;
    air_attack_ani->looping = true;
    air_attack_ani->timedependent = true;

    
    gs->player_animations[player_die].tex = tex->die;


    gs->current_player_anim_name = player_idle;

    anim* en_idle = &gs->enemy_animations[enemy_idle];
    en_idle->tex = tex->enemy_idle;
    en_idle->framecount = 11;
    en_idle->frameduration = .08f;
    en_idle->frameHeight = tex->enemy_idle.height;
    en_idle->frameWidth = tex->enemy_idle.width/en_idle->framecount;
    en_idle->looping = true;
    en_idle->timedependent = true;
    
    anim* en_attack = &gs->enemy_animations[enemy_attack];
    en_attack->tex = tex->enemy_attack;
    en_attack->framecount = 18;
    en_attack->frameduration = .08f;
    en_attack->frameHeight = tex->enemy_attack.height;
    en_attack->frameWidth = tex->enemy_attack.width/en_attack->framecount;
    en_attack->looping = true;
    en_attack->timedependent = true;

    anim* en_run = &gs->enemy_animations[enemy_running];
    en_run->tex = tex->enemy_run;
    en_run->framecount = 13;
    en_run->frameduration = .08f;
    en_run->frameHeight = tex->enemy_run.height;
    en_run->frameWidth = tex->enemy_run.width/en_run->framecount;
    en_run->looping=true;
    en_run->timedependent = true;
    


}
void updateAnimation(GS* gs,float dt){
    anim* a = &gs->player_animations[gs->current_player_anim_name];
    if(!gs->player_animations[gs->current_player_anim_name].timedependent) return;

    gs->player_animations[gs->current_player_anim_name].frametimer += dt;

    while(gs->player_animations[gs->current_player_anim_name].frametimer>=gs->player_animations[gs->current_player_anim_name].frameduration){
        gs->player_animations[gs->current_player_anim_name].frametimer-=gs->player_animations[gs->current_player_anim_name].frameduration;
        if(gs->player_animations[gs->current_player_anim_name].looping){
            gs->player_animations[gs->current_player_anim_name].currentframe = (gs->player_animations[gs->current_player_anim_name].currentframe + 1)%(gs->player_animations[gs->current_player_anim_name].framecount);
        }
        else if(a->currentframe < a->framecount-1 ) a->currentframe++;
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
        .width  = a->frameWidth  * SPRITE_SCALE*1.6f,
        .height = a->frameHeight * SPRITE_SCALE*1.6f
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


void drawBackground(GS* gs){
    float groundTop = gs->gchunk[0].groundChunkRect.y;

    for(int i=0;i<BG_LAYER_COUNT;i++){
        parallax_layer *l = &gs->bgLayers[i];
        float texWidth  = (i==BG_LAYER_COUNT-1)?l->tex.width: l->tex.width  * BG_SCALE;
        float texHeight = (i==BG_LAYER_COUNT-1)?l->tex.height:l->tex.height * BG_SCALE;

        float startX = fmodf(l->offsetX, texWidth);
        if (startX > 0) startX -= texWidth;

        int tilesNeeded = (int)(s_width / texWidth) + 2;

        for(int t=0; t<tilesNeeded; t++){
            Rectangle dest = {
                .x = startX + t*texWidth + gs->camera.target.x - gs->camera.offset.x,
                .y = (i==BG_LAYER_COUNT-1)?groundTop-texHeight:0,
                .width  = texWidth,
                .height = texHeight
            };
            DrawTexturePro(l->tex, l->source, dest, (Vector2){0,0}, 0.0f, WHITE);
        }
    }
}

void unloadTexture(tex* tex){
    UnloadTexture(tex->Background);
    UnloadTexture(tex->dash);
    UnloadTexture(tex->die);
    UnloadTexture(tex->idle);
    UnloadTexture(tex->jumpandfall);
    UnloadTexture(tex->running);
    UnloadTexture(tex->attack1); 
    UnloadTexture(tex->Woods_first);
    UnloadTexture(tex->Woods_fourth);
    UnloadTexture(tex->Woods_second);
    UnloadTexture(tex->woods_third);
}

void playerDashUpdate(GS* gs,float dt){
    Player* a = &gs->player;

    if(a->dashcooldowntimer>=0) a->dashcooldowntimer-=dt;

    if(IsKeyPressed(KEY_LEFT_SHIFT) && !a->isdashing && a->dashcooldowntimer<=0 && a->isgrounded){
        a->isdashing = true;
        a->dashcooldowntimer =dash_cooldowntimer;
        a->dashduration = dash_duration;
        a->velocity.x = (a->facing_left)? -dash_speed : dash_speed;
        a->velocity.y = -260.0f;
        setAnimation(gs,player_dash);
    }
    if(a->dashduration>=0){
        a->dashduration-=dt;
        
        if(a->dashduration<=0){
            a->isdashing = false;
            a->velocity.x = 0;
        }
    }
}

Rectangle gethitbox(GS* gs){
    Player* p = &gs->player;
    float hitbox_height = p->height;
    float hitbox_width = 40.0f*SPRITE_SCALE;
    Rectangle hitbox = (Rectangle){
        .height = hitbox_height,
        .width = hitbox_width,
        .x = (p->facing_left)? p->position.x-hitbox_width : p->position.x+hitbox_width,
        .y = p->position.y
    };
    return hitbox;
}

void hitting(GS* gs,float dt){
    Player* p = &gs->player;
    anim* a = &gs->player_animations[gs->current_player_anim_name];
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !p->isattacking && !p->isdashing && p->isgrounded){
        p->isattacking = true;
        p->hitduration = attackduration;
        setAnimation(gs,attack);
    }else if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !p->isattacking && !p->isdashing && !p->isgrounded){
        p->isattacking = true;
        p->hitduration = airattackduration;
        setAnimation(gs,airattack);
    }
    if(p->isattacking){
        p->hitduration-=dt;
        if(attackstartframe<= a->currentframe && a->currentframe <= attackendframe){
            Rectangle hitbox = gethitbox(gs);
            // check collision of hitbox with enemy rectangle then decrease enemy health
        }
        if(p->hitduration<=0){
            p->isattacking = false;
            setAnimation(gs,player_idle);
        }
    }
}

void updateParallax(GS* gs,float cameradeltax){
    for(int i=0;i<BG_LAYER_COUNT;i++){
        gs->bgLayers[i].offsetX -= cameradeltax * gs->bgLayers[i].scrollfactor;
    }
}


void updateHealth(GS* gs, float dt){
    if(gs->player.isDead) return;
    gs->player.health -= HEALTH_DECAY_RATE * dt;
    if(gs->player.health <= 0.0f){
        gs->player.health = 0.0f;
        gs->player.isDead = true;
    }
}