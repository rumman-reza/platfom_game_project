#include"sound.h"
#include<math.h>
void load_audio(GS* gs){
    gs->audio.menuMusic = LoadMusicStream("assets/music/menu_background_music.mp3");
    gs->audio.menuMusic.looping = true;
    gs->audio.gameMusic = LoadMusicStream("assets/music/game_music1.wav");
    gs->audio.gameMusic.looping = true;
    SetMusicVolume(gs->audio.gameMusic,.4f);
    SetMusicPitch(gs->audio.gameMusic,.6f);
    gs->audio.hurt = LoadSound("assets/music/hurt.mp3");
    gs->audio.die = LoadSound("assets/music/die.mp3");
    SetSoundPitch(gs->audio.die,1.2f);
    gs->audio.enemyDie = LoadSound("assets/music/enemy_die.mp3");
    gs->audio.hit = LoadSound("assets/music/enemy_hurt.mp3");
    gs->audio.gameOverSting = LoadSound("assets/music/game_over_sound.mp3");
    gs->audio.player_run = LoadSound("assets/music/running.mp3");
    SetSoundVolume(gs->audio.player_run,.5f);

    gs->audio.swing = LoadSound("assets/music/player_swing.mp3");
    gs->audio.enemy_swing = LoadSound("assets/music/enemy_swing.mp3");
    gs->audio.dash = LoadSound("assets/music/dash.mp3");
    gs->audio.jump = LoadSound("assets/music/jump.mp3");
    SetSoundPitch(gs->audio.jump,1.2f);

    gs->audio.landing = LoadSound("assets/music/landing.mp3");

    gs->audio.enemy_run = LoadSound("assets/music/enemy_run.mp3");
    gs->audio.health_pickup = LoadSound("assets/music/health_pickup.mp3");
}
void updateMusic(GS* gs){
    UpdateMusicStream(gs->audio.menuMusic);
    UpdateMusicStream(gs->audio.gameMusic);
}


#define footstep_interval 0.35f   // seconds between footstep sounds while running


void unloadAudio(GS* gs){
    UnloadMusicStream(gs->audio.menuMusic);
    UnloadMusicStream(gs->audio.gameMusic);

    UnloadSound(gs->audio.hurt);
    UnloadSound(gs->audio.die);
    UnloadSound(gs->audio.enemy_run);
    UnloadSound(gs->audio.player_run);
}

// player: only step while actually running on the ground
void playerFootstepUpdate(GS* gs, float dt){
    Player* p = &gs->player;
    bool isRunning = p->isgrounded && !p->isDead && !p->isdashing
                      && fabsf(p->velocity.x) > 10.0f;   // same threshold setplayerstate uses for "running"

    if(!isRunning){
        gs->audio.footstepTimer = footstep_interval;   // primes the next step to fire instantly
        return;
    }

    gs->audio.footstepTimer += dt;
    if(gs->audio.footstepTimer >= footstep_interval){
        gs->audio.footstepTimer -= footstep_interval;
        PlaySound(gs->audio.player_run);
    }
}

// enemies: same idea, per-enemy timer since several can walk at once
void enemyFootstepUpdate(GS* gs, float dt){
    for(int i=0;i<max_enemy_num;i++){
        Enemy* e = &gs->enemy[i];
        if(!e->isactive || e->state != walking_enemy){
            gs->audio.enemyFootstepTimer[i] = 0.0f;
            continue;
        }
        gs->audio.enemyFootstepTimer[i] += dt;
        if(gs->audio.enemyFootstepTimer[i] >= footstep_interval){
            gs->audio.enemyFootstepTimer[i] -= footstep_interval;
            PlaySound(gs->audio.enemy_run);
        }
    }
}