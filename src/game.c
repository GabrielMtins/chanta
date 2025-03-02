#include "game.h"
#include "render.h"
#include "scene.h"

Game Game_Create(Context *context, Scene *scene, Resource *resources){
	Game game;

	game.context = context;
	game.resources = resources;
	game.main_scene = scene;

	Scene_Reset(scene, &game);

	return game;
}

bool Game_Run(Game *game){
	bool success = true;

	while(!game->context->quit){
		Context_PollEvent(game->context);

		Scene_Update(game->main_scene);

		Render_Clear(game->context, 0, 0, 0, 255);
		Render_Present(game->context);
	}

	return success;
}

