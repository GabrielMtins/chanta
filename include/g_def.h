#ifndef G_DEF_H
#define G_DEF_H

#define MAX_ENTITIES 256

#define WORLD_WIDTH 256
#define WORLD_HEIGHT 256
#define WORLD_TILE_WIDTH 16
#define WORLD_TILE_HEIGHT 16
#define WORLD_NUM_LAYERS 3
#define WORLD_TILE_OUT_OF_BOUNDS 0
#define WORLD_DATA_SIZE (WORLD_WIDTH * WORLD_HEIGHT * WORLD_NUM_LAYERS)

#define MAX_MEGA_TEXTURES 4
#define MAX_TEXTURES 64
#define MAX_MUSIC 4
#define MAX_SFX 16

enum WORLD_LAYERS_ENUM {
	WORLD_LAYER_BACKGROUND = 0,
	WORLD_LAYER_FOREGROUND = 1,
	WORLD_LAYER_DETAIL = 2
};

#endif
