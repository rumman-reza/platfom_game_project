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
#define pSpeedAir 750.0f
#define jumpSpeed 700.0f
#define gravity 1400.0f
#define dash_speed 2200.0f // jore laaf dewar speed
//different timers
#define dash_duration .45f
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
#define MaxChunkNum 50
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
#define player_invul_time .04f
#define player_real_width 17.0f
#define player_real_height 32.0f


// obstacles and traps
#define pattern_tile_width 182.0f
#define platform_width 368.0f
#define patternheight 40.0f
#define gap_height 80.0f
#define max_spikes 30
#define ground_y  (GetScreenHeight()*3.7f/4)
#define spike_width 92.0f
// for score
#define MAX_HIGH_SCORES   5
#define HIGHSCORE_FILE    "highscore.txt"
#define SCORE_PER_DISTANCE 0.0025f   
#define MAX_NAME_LEN 25

//bomb
#define max_bombs 20
#define bomb_damage 30.0f
#define bomb_width 80.0f  
#define bomb_height 80.0f

#define STARTING_TIMER 1.00f

#define enemy_aggro_range    800.0f   // koto dur theke enemy dhorte ashbe
#define enemy_despawn_margin 300.0f   // koto dur gele enemy despawn hoye jabe

// fog visuals (not gameplay hitbox — see getPgasRect/move_pgas)
#define fog_color_r        40
#define fog_color_g        90
#define fog_color_b        40
#define fog_base_alpha     160
#define fog_top_gap        80.0f   // empty space kept clear at the top of the screen
#define fog_bottom_gap     100.0f   // empty space kept clear above the very bottom
#define fog_vfade          80.0f    // how many px the top/bottom edges fade over
#define fog_edge_fade_width 200.0f  // how many px the leading (right) edge fades over
#define fog_edge_strips    12       // smoothness of the leading-edge fade
#define pgas_max_lag      3000.0f   // gap distance that triggers a snap-forward
#define pgas_teleport_lag  900.0f   // distance left of the player the gas snaps to

#define pgas_player_margin 250.0f   // how far past the player's right edge the fog should reach

#define bomb_explosion_range     250.0f   // damage radius in px, independent of the bomb's own rect
#define bomb_explosion_scale     4.0f     // how big the explosion sprite is drawn (tune to your sheet)
#define max_explosions           20
#define explosion_framecount     8        // SET THIS to your sprite sheet's actual frame count
#define explosion_frameduration  0.05f
#define bomb_fuse_time  .6f   // seconds from entering the blast radius to detonation

#define enemy_healthbar_width   60.0f
#define enemy_healthbar_height  8.0f
#define enemy_healthbar_yoffset 14.0f   // gap between the bar and the top of the sprite

#define max_health_drops       20
#define enemy_health_drop_amount 15.0f   // less than the normal pickup's 25
#define health_drop_width      50.0f
#define health_drop_height     50.0f

#define difficulty_ramp_distance 80000.0f

#define tutorial_char_interval 0.03f   // seconds per revealed character
#define tutorial_fade_speed    1.5f    // alpha change per second

#define bush_sprite_count   6  
#define detail_sprite_count 9   

#define max_bush_decor       400
#define bush_spacing         55.0f    // px between bush pieces — TUNE to your sprite's actual on-screen width so they touch with no gaps
#define bush_min_scale       2.5f
#define bush_max_scale       3.5f

#define max_detail_decor        200
#define detail_cluster_chance   55       // percent chance per plain tile to start a cluster
#define detail_cluster_min      2
#define detail_cluster_max      5
#define detail_cluster_spacing  50.0f
#define detail_min_scale        1.8f
#define detail_max_scale        3.0f
#define skip_timer 2.0f

#endif  