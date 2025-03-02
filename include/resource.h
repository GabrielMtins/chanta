#ifndef RESOURCE_H
#define RESOURCE_H

#include "texture.h"

#define MAX_MEGA_TEXTURES 4
#define MAX_TEXTURES 64

typedef struct {
	MegaTexture mega_textures[MAX_MEGA_TEXTURES];
	Texture textures[MAX_TEXTURES];
} Resource;

#endif
