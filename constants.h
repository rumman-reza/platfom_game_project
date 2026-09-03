#ifndef CONSTANTS_H
#define CONSTANTS_H

#include"raylib.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

//bg layers
#define BG_LAYER_COUNT 6
//screen er size newa
#define s_height GetScreenHeight()
#define s_width GetScreenWidth()

#define camera_half_deadzone 450.0f // player ke koto tuku cholar jaiga dewa hobe camerar moddhe

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

// enemy er jono
#define enSpeed 1000.0f
#define encooldown 0.96f
#define attackrange 90.0f
// health  maximum and koto kore kombe seta 
#define PLAYER_MAX_HEALTH 100.0f
#define HEALTH_DECAY_RATE 5.0f 


#endif