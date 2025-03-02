#ifndef GAME_H
#define GAME_H

#include "context.h"
#include "g_types.h"

Game Game_Create(Context *context);

bool Game_Run(Game *game);

#include "scene.h"

#endif
