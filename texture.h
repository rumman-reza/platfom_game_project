#ifndef TEXTURE_H
#define TEXTURE_H

#include"types.h"

void loadTexture(tex* tex, GS* gs);
Texture2D LoadPixelTexture(const char *path);
void unloadTexture(tex* tex);


#endif