#ifndef SOUND_H
#define SOUND_H
#include"types.h"

void load_audio(GS* gs);
void updateMusic(GS* gs); 
void unloadAudio(GS* gs);
void playerFootstepUpdate(GS* gs, float dt);
void enemyFootstepUpdate(GS* gs, float dt);
#endif