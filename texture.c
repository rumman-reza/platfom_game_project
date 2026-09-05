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
    tex->enemy_dead = LoadPixelTexture("assets/enemy_sprites/SkeletonDead.png");
    tex->enemy_hurt = LoadPixelTexture("assets/enemy_sprites/SkeletonHit.png");

    gs->cfonts.menu_font1 = LoadFontEx("assets/fonts/Pixelmania.ttf",200,0,0);

}