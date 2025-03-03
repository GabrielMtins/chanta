#ifndef SCENE_H
#define SCENE_H

#include "g_types.h"
#include "mems.h"

Scene *Scene_Setup(void);

bool Scene_Reset(Scene *scene, Game *game);

bool Scene_Update(Scene *scene);

bool Scene_SetWorldTexture(Scene *scene, Texture *texture);

Entity * Scene_AddEntity(Scene *scene);

bool Scene_RemoveEntity(Scene *scene, Entity *entity);

#endif
