#ifndef GROUND_H
#define GROUND_H
#include"types.h"

void updateGround(GS *gs);
void groundedCheck(GS* gs);
void pushgroundchunk(GS* gs,float x,float y,float height,float width,bool has_health_iteam);
void addspike(GS* gs,float x,float y,float height,float width);
void drawSpikes(GS* gs);
Rectangle getGroundcheckRec(GS* gs);
void spawn_healthrect(GS* gs,float x,float y,float width);

void addbomb(GS* gs, float x, float y, float width, float height);
void drawBombs(GS* gs, tex* textures);


#endif