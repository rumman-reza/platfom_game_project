    #include"ground.h"
    #include"player.h"
    #include"pattern.h"
    #include"types.h"
    #include <stdlib.h> 
    #include"game.h"
    #include"sound.h"   
    #include<math.h>

    Rectangle getGroundcheckRec(GS* gs){
        Rectangle player = getPlayerRect(gs);
        Rectangle groundcheckrec = (Rectangle){
                .height = 2.0f,
                .width = player.width/2.0f,
                .x= player.x+player.width/4.0f,
                .y = player.y + player.height
        };
        return groundcheckrec;
    }

    float getDifficultyFactor(GS* gs){
        float t = gs->distance_traveled / difficulty_ramp_distance;
        if(t > 1.0f) t = 1.0f;
        if(t < 0.0f) t = 0.0f;
        return t;
    }

    void groundedCheck(GS* gs){
        Player* p = &gs->player;
        bool wasGrounded = p->isgrounded;
        float fallSpeed  = p->velocity.y;   // capture before the loop zeroes it out on landing

        p->isgrounded = false;
        for(int i = 0; i < MaxChunkNum; i++){
            if(CheckCollisionRecs(getGroundcheckRec(gs), gs->gchunk[i].groundChunkRect)){
                p->isgrounded = true;
                p->position.y = gs->gchunk[i].groundChunkRect.y - p->collisionOffset.y - p->height;
                p->velocity.y = 0;
                break;
            }
        }

        if(p->isgrounded && !wasGrounded && fallSpeed > 50.0f){
            PlaySound(gs->audio.landing);
        }
    }
    void pushgroundchunk(GS *gs,float x,float y,float height,float width,bool has_health_item)
    {
        int index = gs->chunk_index;

        gs->gchunk[index].groundChunkRect =
            (Rectangle){
                .x = x,
                .y = y,
                .width = width,
                .height = height
            };

            
            
        gs->chunk_index = (gs->chunk_index + 1) % MaxChunkNum;
    }

    void spawn_healthrect(GS* gs,float x,float y,float width){
        int index = gs->chunk_index;
        
        gs->gchunk[index].hasHealthItem = true;

        gs->gchunk[index].healthItemCollected = false;

        gs->gchunk[index].healthItemRect = (Rectangle){
            .x = x + width * 0.5f - 25.0f,
            .y = y - 50.0f,
            .width = 50.0f,
            .height = 50.0f
        };
    }


    void addspike(GS* gs,float x,float y,float width,float height)
    {
        int index = gs->spike_index;

        gs->spikes[index].rect =
            (Rectangle){
                .x = x,
                .y = y,
                .width = width,
                .height = height
            };

        gs->spikes[index].isactive = true;

        gs->spike_index =
            (gs->spike_index + 1) % max_spikes;
    }


    void updateGround(GS* gs)
    {
        float view_distance =
            gs->player.position.x + s_width;

        while(gs->next_spawn_point < view_distance)
        {
            float distSincePattern = gs->next_spawn_point - gs->lastPatternEndX;

            if(distSincePattern >= gs->gapBetweenTheNextPattern)
            {
                const pattern *p;
                if(gs->distance_traveled<40000)  p = &all_easy_patterns[GetRandomValue(0, pattern_count1 - 1)];
                else p = &all_medium_patterns[GetRandomValue(0, pattern_count2 - 1)];

                spawn_pattern(gs,p,gs->next_spawn_point,ground_y,s_height);

                
                float patternWidth =getPatternWidth(p) * pattern_tile_width;
                
                gs->next_spawn_point += patternWidth;
                
                gs->lastPatternEndX = gs->next_spawn_point;
                
                spawnBushLine(gs, gs->next_spawn_point, ground_y); 

                float diff = getDifficultyFactor(gs);
                
                int minGap = (int)(1600 - 800*diff);   // 1600 -> 800
                int maxGap = (int)(2500 - 1000*diff);  // 2500 -> 1500
                gs->gapBetweenTheNextPattern = (float)GetRandomValue(minGap, maxGap);
            }
            else
            {
                pushgroundchunk(gs,gs->next_spawn_point,ground_y,s_height - ground_y,platform_width, true);

                
                if (GetRandomValue(1, 100) <= 10) { 
                    
                    float bombX = gs->next_spawn_point + (float)GetRandomValue(0, s_width - bomb_width);
                    float bombY = ground_y - bomb_height;
                    addbomb(gs, bombX, bombY, bomb_width, bomb_height);
                }

                 if (GetRandomValue(1, 100) <= detail_cluster_chance) {
                    float startX = gs->next_spawn_point + (float)GetRandomValue(0, (int)platform_width);
                    spawnDetailCluster(gs, startX, ground_y);
                }

                gs->next_spawn_point += platform_width;
                spawnBushLine(gs, gs->next_spawn_point, ground_y); 
            }
        }
    }

    void drawSpikes(GS* gs)
    {
        for(int i = 0; i < max_spikes; i++)
        {
            if(!gs->spikes[i].isactive)
                continue;

            Rectangle r = gs->spikes[i].rect;
            
            Rectangle source = {
            .x = 0,
            .y = 0,
            .width  = 16,
            .height = 16
        };
        Rectangle dest = {
            .x = r.x,
            .y = r.y,
            .width  = r.width,
            .height = r.height
        };
        DrawTexturePro(gs->spikes[i].spike_sprite, source, dest, (Vector2){0,0}, 0.0f, WHITE);
            
        }
    }



    void addbomb(GS* gs, float x, float y, float width, float height) {
        int index = gs->bomb_index;
        gs->bombs[index].rect = (Rectangle){x, y, width, height};
        gs->bombs[index].isactive = true;
        gs->bombs[index].armed = false;
        gs->bombs[index].fuseTimer = 0.0f;
        gs->bomb_index = (gs->bomb_index + 1) % max_bombs;
    }

    void drawBombs(GS* gs, tex* textures) {
        for (int i = 0; i < max_bombs; i++) {
            if (!gs->bombs[i].isactive) continue;

            Rectangle r = gs->bombs[i].rect;
            Rectangle source = {0, 0, textures->bomb_sprite.width, textures->bomb_sprite.height};
            Rectangle dest = {r.x, r.y, r.width, r.height};

            DrawTexturePro(textures->bomb_sprite, source, dest, (Vector2){0,0}, 0.0f, WHITE);

            if (gs->bombs[i].armed) {
                float t = 1.0f - (gs->bombs[i].fuseTimer / bomb_fuse_time);  // 0 at arm, 1 near detonation
                float pulseSpeed = 8.0f + t * 20.0f;                        // pulses faster as fuse runs out
                float pulse = (sinf((float)GetTime() * pulseSpeed) + 1.0f) / 2.0f;  // 0..1
                unsigned char alpha = (unsigned char)(pulse * 200);

                BeginBlendMode(BLEND_ADDITIVE);
                DrawTexturePro(textures->bomb_sprite, source, dest, (Vector2){0,0}, 0.0f, (Color){255,255,255,alpha});
                EndBlendMode();
            }
        }
    }


static void drawDecorPiece(Texture2D t, ForegroundDecor* d){
    float drawW = t.width  * d->scale;
    float drawH = t.height * d->scale;

    Rectangle source = {
        .x = 0, .y = 0,
        .width  = d->facing_left ? -(float)t.width : (float)t.width,
        .height = (float)t.height
    };
    Rectangle dest = {
        .x = d->position.x - drawW/2.0f,
        .y = d->position.y - drawH,
        .width  = drawW,
        .height = drawH
    };
    DrawTexturePro(t, source, dest, (Vector2){0,0}, 0.0f, WHITE);
}
void spawnBushLine(GS* gs, float toX, float groundY){
    while(gs->last_bush_x < toX){
        int index = gs->bushDecor_index;
        ForegroundDecor* d = &gs->bushDecor[index];

        d->position = (Vector2){ gs->last_bush_x, groundY };
        d->spriteIndex = GetRandomValue(0, bush_sprite_count - 1);
        d->scale = (float)GetRandomValue((int)(bush_min_scale*10), (int)(bush_max_scale*10)) / 10.0f;
        d->facing_left = GetRandomValue(0,1) == 1;
        d->active = true;

        gs->bushDecor_index = (index + 1) % max_bush_decor;
        gs->last_bush_x += bush_spacing;
    }
}
void spawnDetailCluster(GS* gs, float startX, float groundY){
    int spriteIndex = GetRandomValue(0, detail_sprite_count - 1);
    bool facing_left = GetRandomValue(0,1) == 1;
    int count = GetRandomValue(detail_cluster_min, detail_cluster_max);

    for(int i=0;i<count;i++){
        float x = startX + i * detail_cluster_spacing + (float)GetRandomValue(-10,10);

        int index = gs->detailDecor_index;
        ForegroundDecor* d = &gs->detailDecor[index];

        d->position = (Vector2){ x, groundY };
        d->spriteIndex = spriteIndex;
        d->facing_left = facing_left;
        d->scale = (float)GetRandomValue((int)(detail_min_scale*10), (int)(detail_max_scale*10)) / 10.0f;
        d->active = true;

        gs->detailDecor_index = (index + 1) % max_detail_decor;
    }
}
void drawBushLine(GS* gs, tex* textures){
    for(int i=0;i<max_bush_decor;i++){
        ForegroundDecor* d = &gs->bushDecor[i];
        if(!d->active) continue;
        drawDecorPiece(textures->bush_sprites[d->spriteIndex], d);
    }
}

void drawDetailDecor(GS* gs, tex* textures){
    for(int i=0;i<max_detail_decor;i++){
        ForegroundDecor* d = &gs->detailDecor[i];
        if(!d->active) continue;
        drawDecorPiece(textures->detail_sprites[d->spriteIndex], d);
    }
}