#include <stdio.h>
#include "context.h"
#include "game.h"
#include "mems.h"
#include "scene.h"
#include "entity.h"

#define MEMORY_USED 1024 * 1024 * 8

int main(int argc, char **argv){
	Context *context;
	Game *game;
	Mems memory;

	(void) argc;
	(void) argv;

	/* Criar a estrutura que organizará a memória */
	memory = Mems_Create(malloc(MEMORY_USED), MEMORY_USED);

	context = Context_Create("hi", 480, 272, &memory);
	Context_SetDataFromFile(context, "tilemap.png");
	game = Game_Create(context);

	printf("%i\n", MegaTexture_LoadData(&game->resources->mega_textures[0], context, 0));

	Texture_Create(
			&game->resources->textures[0], 
			&game->resources->mega_textures[0],
			0,
			0,
			256,
			256,
			WORLD_TILE_WIDTH,
			WORLD_TILE_HEIGHT
			);

	Scene_SetWorldTexture(game->main_scene, Game_GetTexture(game, 0));
	Scene_SetHudTexture(game->main_scene, Game_GetTexture(game, 0));

	Entity *ent = Scene_AddEntity(game->main_scene);
	Entity_Reset(ent);

	ent->texture = &game->resources->textures[0];
	ent->cell_id = 2;
	ent->position = (Vec2){65.0f, 0.0f};
	ent->velocity = (Vec2){20.01f, 60.0f};
	ent->size = (Vec2){24.0f, 24.0f};
	ent->collision_mask = 1;

	Game_Run(game);

	MegaTexture_Destroy(&game->resources->mega_textures[0]);
	Context_Destroy(context);
	free(memory.block);

	printf("%lu\n", memory.top);

	return 0;
}
