#include "game.h"
#include "render.h"

Game Game_Create(Context *context){
	Game game;

	game.context = context;

	return game;
}

bool Game_Run(Game *game){
	bool success = true;

	while(!game->context->quit){
		Context_PollEvent(game->context);

		Render_Clear(game->context, 0, 0, 0, 255);
		Render_Present(game->context);
	}

	return success;
}

