#ifndef SCENE_H
#define SCENE_H

#include "g_types.h"
#include "mems.h"

Scene *Scene_Setup(void);

bool Scene_Reset(Scene *scene, Game *game);

bool Scene_Update(Scene *scene);

bool Scene_Render(Scene *scene);

bool Scene_SetWorldTexture(Scene *scene, Texture *texture);

bool Scene_SetHudTexture(Scene *scene, Texture *texture);

uint8_t Scene_GetTileId(Scene *scene, int x, int y, int layer);

bool Scene_SetTileId(Scene *scene, int x, int y, int layer, uint8_t id);

Entity * Scene_AddEntity(Scene *scene);

bool Scene_RemoveEntity(Scene *scene, Entity *entity);

#endif
