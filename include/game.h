#ifndef GAME_H
#define GAME_H

#include "context.h"

typedef struct scene_s Scene;

typedef struct game_s {
	Context *context;
	Scene main_scene;
} Game;

Game Game_Create(Context *context);

bool Game_Run(Game *game);

#endif
