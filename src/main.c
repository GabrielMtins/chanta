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
			32,
			32
			);

	Entity *ent = Scene_AddEntity(game->main_scene);
	Entity_Reset(ent);

	ent->texture = &game->resources->textures[0];
	ent->cell_id = 0;
	ent->position = (Vec2){210.0f, -20.0f};
	ent->velocity = (Vec2){-30.0f, 80.0f};
	ent->size = (Vec2){32.0f, 32.0f};
	ent->collision_mask = 1;

	ent = Scene_AddEntity(game->main_scene);
	Entity_Reset(ent);

	ent->texture = &game->resources->textures[0];
	ent->cell_id = 0;
	ent->position = (Vec2){128.0f, 128.0f};
	ent->size = (Vec2){32.0f, 32.0f};
	//ent->collision_layer = 1;

	//ent->velocity = (Vec2){-10.0f, 0.0f};

	Game_Run(game);

	Context_Destroy(context);
	free(memory.block);

	printf("%lu\n", memory.top);

	return 0;
}
