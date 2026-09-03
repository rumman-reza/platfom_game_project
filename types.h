#ifndef TYPES_H
#define TYPES_H
#include"raylib.h"
#include"constants.h"

// menu ar game e switch korar jonno enum
typedef enum gameScreen{
    MENU=1,GAME=0
} gamescreen;

// ground er jonno struct
typedef struct groundChunk
{
    Rectangle groundChunkRect;

    bool hasHealthItem;         // chunk e health ache kina seta dekhbe 
    Rectangle healthItemRect;   // health item er shape 
    bool healthItemCollected;   // item ta ki already picked or not
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

// attack related variables
    bool isattacking;
    float hitduration;
    float invultimer;
    bool hashitthiswing;

    // health  variable gulo 
    float health;  
    float maxHealth;   //maximum health koto
    bool isDead;

}Player;


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
    
    //enemy things
    // enemy sobshomoy screen e present thake na tai enemy gs e thakbe na, function call kore use korte hobe
    
}GS;

typedef enum enemystate{
    idle_enemy,
    walking_enemy,
    attacking_enemy,
    hurting_enemy,
    dead_enemy
} enemystate;

typedef struct Enemy{
    float width;
    float height;
    Vector2 position;
    Vector2 velocity;
    Vector2 inital_position;
    bool facing_left;
    bool isgrounded;
    anim enemy_animations[enemy_anim_num];
    anim_name current_enemy_anim_name;
    enemystate state;
    bool isactive;
    
    float health;
    float maxhealth;
    bool isdead;
    bool isattacking;
    float attack_duration;
    float attack_cooldown;
    bool hashitplayerthisswing;
    
} Enemy;

typedef struct HealthItem {
    Vector2 position;
    float width;
    float height;
    float healAmount; // koto kore health barbe seta 
    bool active;      // healing obj screen e ache naki chole gese 
} HealthItem;

#endif