#include <stdio.h>
#include "context.h"
#include "game.h"
#include "mems.h"
#include "scene.h"
#include "entity.h"

#define MEMORY_USED 1024 * 1024 * 8

void ent_update(Scene *scene, Entity *entity, float dt);

void ent_update(Scene *scene, Entity *entity, float dt){
	Vec2 gravity = {0.0f, 80.0f};
	Vec2 d_gravity;
	float run = 0.0f;
	const uint8_t *keys = SDL_GetKeyboardState(NULL);

	Vec2_Mul(&gravity, &gravity, dt);

	if(keys[SDL_SCANCODE_A])
		run -= 1.0f;

	if(keys[SDL_SCANCODE_D])
		run += 1.0f;

	if(keys[SDL_SCANCODE_SPACE] && entity->velocity.y == 0.0f)
		entity->velocity.y -= 100.0f;

	entity->velocity.x = run * 50.0f;
	Vec2_Add(&entity->velocity, &entity->velocity, &gravity);

}

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

	MegaTexture_Load(&game->resources->mega_textures[0], context, "tilemap.png");

	Texture_Create(
			&game->resources->textures[0], 
			&game->resources->mega_textures[0],
			0,
			0,
			64,
			16,
			WORLD_TILE_WIDTH,
			WORLD_TILE_HEIGHT
			);

	Texture_Create(
			&game->resources->textures[1], 
			&game->resources->mega_textures[0],
			0,
			16,
			64,
			48,
			16,
			24
			);

	Scene_SetWorldTexture(game->main_scene, Game_GetTexture(game, 0));
	Scene_SetHudTexture(game->main_scene, Game_GetTexture(game, 0));

	for(int i = 0; i < 10; i++)
		Scene_SetTileId(game->main_scene, i, 10, WORLD_LAYER_FOREGROUND, 1);

	Entity *ent = Scene_AddEntity(game->main_scene);
	Entity_Reset(ent);

	ent->texture = Game_GetTexture(game, 1);
	ent->cell_id = 0;
	ent->position = (Vec2){65.0f, 15.0f};
	ent->size = (Vec2){24.0f, 24.0f};
	ent->collision_mask = 1;
	ent->update = ent_update;

	Game_Run(game);

	MegaTexture_Destroy(&game->resources->mega_textures[0]);
	Context_Destroy(context);
	free(memory.block);

	printf("%lu\n", memory.top);

	return 0;
}
