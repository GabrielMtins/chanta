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
	game = Game_Create(context);

	printf("%i\n", MegaTexture_Load(&game->resources->mega_textures[0], context, "tilemap.png"));

	Texture_Create(
			&game->resources->textures[0], 
			&game->resources->mega_textures[0],
			0,
			0,
			256,
			256,
			16,
			16
			);

	Scene_SetWorldTexture(game->main_scene, Game_GetTexture(game, 0));

	Entity *ent = Scene_AddEntity(game->main_scene);
	Entity_Reset(ent);

	ent->texture = &game->resources->textures[0];
	ent->cell_id = 2;
	ent->position = (Vec2){210.0f, 0.0f};
	ent->velocity = (Vec2){30.01f, 0.0f};
	ent->size = (Vec2){16.0f, 16.0f};
	ent->collision_mask = 1;

	Game_Run(game);

	Context_Destroy(context);
	free(memory.block);

	printf("%lu\n", memory.top);

	return 0;
}
