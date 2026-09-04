#ifndef ANIMATION_H
#define ANIMATION_H

#include "types.h"


void loadAnimation(GS* gs,tex* tex, anim* anim);
void updateAnimation(anim* a,float dt);
#endif