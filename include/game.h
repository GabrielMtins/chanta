#ifndef GAME_H
#define GAME_H

#include "context.h"
#include "g_types.h"

Game Game_Create(Context *context, Scene *scene, Resource *resources);

bool Game_Run(Game *game);

#endif
