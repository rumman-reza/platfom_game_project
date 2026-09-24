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
#define pAttackMoveSpeed 700.0f
#define pSpeedAir 1000.0f
#define jumpSpeed 700.0f
#define gravity 1400.0f
#define dash_speed 2000.0f // jore laaf dewar speed
//different timers
#define dash_duration .32f
#define dash_cooldowntimer .6f
#define attackduration .54f
#define airattackduration .56f
// texture choto boro korar jonno
#define SPRITE_SCALE 3.0f
#define BG_SCALE (SPRITE_SCALE * 1.3f)
// attack koto tuku gulo frame er moddhe hobe 
#define attackstartframe 3
#define attackendframe 6    
//koto gulo ground chunk dekhabe
#define MaxChunkNum 30
#define player_attack_power 30.0f

// enemy er jono
#define enSpeed 900.0f
#define encooldown 0.96f
#define attackrange 120.0f
#define enemy_attack_power 5.0f
#define max_enemy_num 100
#define enemy_max_health 60.0f
#define enemy_invultimer .32f
#define enemy_attack_start_frame 6
#define enemy_attack_end_frame 10
#define spikecooldown .60f
#define spike_damage 25.0f

// health  maximum and koto kore kombe seta 
#define PLAYER_MAX_HEALTH 100.0f
#define HEALTH_DECAY_RATE 4.0f 
#define player_invul_time .08f
#define player_real_width 17.0f
#define player_real_height 32.0f


// obstacles and traps
#define pattern_tile_width 80.0f
#define platform_width 180.0f
#define patternheight 40.0f
#define gap_height 80.0f
#define max_spikes 30
#define ground_y  (GetScreenHeight()*3.7f/4)

// for score
#define MAX_HIGH_SCORES   5
#define HIGHSCORE_FILE    "highscore.txt"
#define SCORE_PER_DISTANCE 0.1f   
#define MAX_NAME_LEN 25

//bomb
#define max_bombs 20
#define bomb_damage 50.0f
#define bomb_width 40.0f  
#define bomb_height 40.0f


#endif  