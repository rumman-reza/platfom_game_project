#include"background.h"
#include<math.h>

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

void updateParallax(GS* gs,float cameradeltax){
    for(int i=0;i<BG_LAYER_COUNT;i++){
        gs->bgLayers[i].offsetX -= cameradeltax * gs->bgLayers[i].scrollfactor;
    }
}

void drawBackgroundMenu(GS* gs){
    for(int i = 0; i < BG_LAYER_COUNT; i++){
        parallax_layer *l = &gs->bgLayers[i];
        float texWidth  = (i == BG_LAYER_COUNT-1) ? l->tex.width  : l->tex.width  * BG_SCALE;
        float texHeight = (i == BG_LAYER_COUNT-1) ? l->tex.height : l->tex.height * BG_SCALE;

        float startX = fmodf(l->offsetX, texWidth);
        if (startX > 0) startX -= texWidth;

        int tilesNeeded = (int)(s_width / texWidth) + 2;

        for(int t = 0; t < tilesNeeded; t++){
            Rectangle dest = {
                .x = startX + t * texWidth,
                .y = (i == BG_LAYER_COUNT-1) ? s_height - texHeight : 0, // menu-te ground chunk nei, tai screen bottom use korlam
                .width  = texWidth,
                .height = texHeight
            };
            DrawTexturePro(l->tex, l->source, dest, (Vector2){0,0}, 0.0f, WHITE);
        }
    }
}