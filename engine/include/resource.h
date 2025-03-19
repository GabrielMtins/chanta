#ifndef RESOURCE_H
#define RESOURCE_H

#include "texture.h"
#include "sfx.h"

typedef struct {
	MegaTexture mega_textures[MAX_MEGA_TEXTURES];
	Texture textures[MAX_TEXTURES];
	Sfx sfx[MAX_SFX];
	Music musics[MAX_MUSIC];
} Resource;

#endif
