#ifndef RESOURCE_H
#define RESOURCE_H

#include "texture.h"

typedef struct {
	MegaTexture mega_textures[MAX_MEGA_TEXTURES];
	Texture textures[MAX_TEXTURES];
} Resource;

#endif
