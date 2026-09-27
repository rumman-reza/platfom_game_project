#include "texture.h"

Texture2D LoadPixelTexture(const char *path) {
    Texture2D t = LoadTexture(path);
    SetTextureFilter(t, TEXTURE_FILTER_POINT);
    return t;
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
    UnloadTexture(tex->Bush_background);

    UnloadTexture(tex->health_item);

    for(int i=0;i<bush_sprite_count;i++)   UnloadTexture(tex->bush_sprites[i]);
    for(int i=0;i<detail_sprite_count;i++) UnloadTexture(tex->detail_sprites[i]);
    // UnloadFont(gs->cfonts.menu_font3);
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
    
    tex->spike_sprite = LoadPixelTexture("assets/sprites/spike_sprite.png");

    tex->enemy_idle = LoadPixelTexture("assets/enemy_sprites/SkeletonIdle.png");
    tex->enemy_run = LoadPixelTexture("assets/enemy_sprites/SkeletonWalk.png");
    tex->enemy_attack = LoadPixelTexture("assets/enemy_sprites/SkeletonAttack.png");
    tex->enemy_dead = LoadPixelTexture("assets/enemy_sprites/SkeletonDead.png");
    tex->enemy_hurt = LoadPixelTexture("assets/enemy_sprites/SkeletonHit.png");

    gs->cfonts.menu_font1 = LoadFontEx("assets/fonts/Pixelmania.ttf",200,0,0);
    gs->cfonts.menu_font2 = LoadFontEx("assets/fonts/StayPixelDEMO.ttf",200,0,0);
    gs->cfonts.menu_font3 = LoadFontEx("assets/fonts/BoldPixels.ttf", 200, 0, 0);

    for(int i=1;i<=12;i++){
        gs->pgas.pgas_anim[i-1] = LoadPixelTexture(TextFormat("assets/PNG/Posionous_Smoke_Frame_%d.png",i));
    }
    
    for(int i=0;i<max_spikes;i++){
        gs->spikes[i].spike_sprite = LoadPixelTexture("assets/sprites/spike_sprite.png");
    }
    tex->health_item = LoadPixelTexture("assets/sprites/health_item.png");
   
    tex->bomb_sprite = LoadPixelTexture("assets/sprites/bomb.png");
    for(int i=0;i<MaxChunkNum;i++) gs->gchunk[i].texture = LoadPixelTexture("assets/background_elements/floating_platform.png");

    tex->bomb_explosion = LoadPixelTexture("assets/sprites/bomb_explosion.png");

    tex->enemy_health_drop = LoadTexture("D:\\programming\\raylib_practise\\assets\\PNG\\enemy_health_drop.png");
    tex->platform_health_drop = LoadTexture("D:\\programming\\raylib_practise\\assets\\PNG\\platform health.png");
    tex->pgas = LoadTexture("assets/PNG/pgas.png");
    tex->Large_gap = LoadTexture("assets\\PNG\\large_gap1.png");
    tex->Large_gap2 = LoadTexture("assets\\PNG\\large_gap_2.png");

    tex->bush_sprites[0] = LoadPixelTexture("assets\\foreground_elements\\BUSH FOREGROUND 1-2.png");
    tex->bush_sprites[1] = LoadPixelTexture("assets\\foreground_elements\\BUSH FOREGROUND 1-3.png");
    tex->bush_sprites[2] = LoadPixelTexture("assets\\foreground_elements\\BUSH FOREGROUND 1-4.png");
    tex->bush_sprites[3] = LoadPixelTexture("assets\\foreground_elements\\BUSH FOREGROUND 1-5.png");
    tex->bush_sprites[4] = LoadPixelTexture("assets\\foreground_elements\\BUSH FOREGROUND 1-6.png");
    tex->bush_sprites[5] = LoadPixelTexture("assets\\foreground_elements\\BUSH FOREGROUND 1-7.png");
    tex->detail_sprites[0] = LoadPixelTexture("assets\\foreground_elements\\GRASS 1-1.png");
    tex->detail_sprites[1] = LoadPixelTexture("assets\\foreground_elements\\GRASS 1-2.png");
    tex->detail_sprites[2] = LoadPixelTexture("assets\\foreground_elements\\GRASS 2-1.png");
    tex->detail_sprites[3] = LoadPixelTexture("assets\\foreground_elements\\GRASS 2-2.png");
    tex->detail_sprites[4] = LoadPixelTexture("assets\\foreground_elements\\GRASS 3-1.png");
    tex->detail_sprites[5] = LoadPixelTexture("assets\\foreground_elements\\GRASS 3-2.png");
    tex->detail_sprites[6] = LoadPixelTexture("assets\\foreground_elements\\MUSHROOM 1-1.png");
    tex->detail_sprites[7] = LoadPixelTexture("assets\\foreground_elements\\MUSHROOM 1-2.png");
    tex->detail_sprites[8] = LoadPixelTexture("assets\\foreground_elements\\MUSHROOM 2-1.png");

}