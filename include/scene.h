#ifndef SCENE_H
#define SCENE_H

typedef struct game_s Game;

typedef struct scene_s {
	Game *game;
} Scene;

bool Scene_Reset(Scene *scene, Game *game);

bool Scene_Update(Scene *scene);

#endif
