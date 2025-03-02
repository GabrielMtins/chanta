#ifndef SCENE_H
#define SCENE_H

#include "g_types.h"
#include "mems.h"

Scene *Scene_Setup(Mems *mems);

bool Scene_Reset(Scene *scene, Game *game);

bool Scene_Update(Scene *scene);

Entity * Scene_AddEntity(Scene *scene);

bool Scene_RemoveEntity(Scene *scene, Entity *entity);

#endif
