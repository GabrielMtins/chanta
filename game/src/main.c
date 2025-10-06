#include <stdio.h>
#include <stdio.h>
#include <time.h>
#include "context.h"
#include "game.h"
#include "mems.h"
#include "scene.h"
#include "entity.h"

#define MEMORY_USED 1024 * 1024 * 8
#define NUM_BOMBS 99

#define TILE_BOMB 10
#define TILE_NONE 11
#define TILE_FLAG 12
#define TILE_SELECTOR 13
#define TILE_A 17
#define TILE_SLASH 10

enum selector_states {
	SELECTOR_STATE_NULL = 0,
	SELECTOR_STATE_START,
	SELECTOR_STATE_MOVING,
	SELECTOR_STATE_MOVING_ON,
	SELECTOR_STATE_LOST
};

static int num_flag = 0;

bool open_tile(Scene *, int, int);
void build_world(Scene *, int, int);
void selector_think(Scene *, Entity *);
void selector_update(Scene *, Entity *, float);

bool check_win_state(Scene *scene){
	int count = 0;

	for(size_t i = 0; i < WORLD_WIDTH; i++){
		for(size_t j = 0; j < WORLD_HEIGHT; j++){
			if(Scene_GetTileId(scene, i, j, WORLD_LAYER_FOREGROUND) == -1)
				count++;
		}
	}

	return count == (WORLD_WIDTH * WORLD_HEIGHT - NUM_BOMBS);
}

void load_next_scene(Scene *scene){
	Scene_Reset(scene, scene->game);

	Scene_SetWorldTexture(scene, Game_GetTexture(scene->game, 0));
	Scene_SetHudTexture(scene, Game_GetTexture(scene->game, 1));

	for(size_t i = 0; i < WORLD_WIDTH; i++){
		for(size_t j = 0; j < WORLD_HEIGHT; j++){ Scene_SetTileId(scene, i, j, WORLD_LAYER_FOREGROUND, TILE_NONE);
		}
	}

	scene->world->no_bounds = false;
	scene->camera.y = -16.0f;

	num_flag = 0;

	Entity *selector = Scene_AddEntity(scene);
	Entity_Reset(selector);
	
	selector->position = (Vec2){(WORLD_WIDTH / 2 - 1) * WORLD_TILE_WIDTH, (WORLD_HEIGHT / 2 - 1) * WORLD_TILE_HEIGHT};
	selector->texture = Game_GetTexture(scene->game, 0);
	selector->cell_id = TILE_SELECTOR;
	selector->think = selector_think;
	selector->update = selector_update;
	selector->next_think = 150;
}

void selector_update(Scene *scene, Entity *entity, float dt){
	(void) dt;
	(void) entity;

	size_t pos = 0;
	size_t seconds = scene->tick / 1000;
	size_t minutes = seconds / 60;
	seconds %= 60;

	const char *flag_str = "flag";
	const char *time = "time";

	while(*flag_str != '\0'){
		Scene_SetHudTile(scene, pos++, 0, *(flag_str++) - 'a' + TILE_A);
	}

	pos += 1;

	Scene_SetHudTile(scene, pos++, 0, num_flag / 10);
	Scene_SetHudTile(scene, pos++, 0, num_flag % 10);

	Scene_SetHudTile(scene, pos++, 0, 11);

	Scene_SetHudTile(scene, pos++, 0, NUM_BOMBS / 10);
	Scene_SetHudTile(scene, pos++, 0, NUM_BOMBS % 10);

	pos = 50;

	while(*time != '\0'){
		Scene_SetHudTile(scene, pos++, 0, *(time++) - 'a' + TILE_A);
	}
	
	pos++;

	Scene_SetHudTile(scene, pos++, 0, minutes / 10);
	Scene_SetHudTile(scene, pos++, 0, minutes % 10);
	Scene_SetHudTile(scene, pos++, 0, 10);
	Scene_SetHudTile(scene, pos++, 0, seconds / 10);
	Scene_SetHudTile(scene, pos++, 0, seconds % 10);
}

void selector_think(Scene *scene, Entity *entity){
	int x, y, dx, dy;
	const uint8_t *keys = SDL_GetKeyboardState(NULL);

	int b_open_tile = 0, b_put_flag = 0;

	x = (int) (entity->position.x / WORLD_TILE_WIDTH);
	y = (int) (entity->position.y / WORLD_TILE_HEIGHT);

	dx = dy = 0;

	b_open_tile = Context_GetKey(scene->game->context, INPUT_B);
	b_put_flag = Context_GetKey(scene->game->context, INPUT_A);

	if(Context_GetKey(scene->game->context, INPUT_START))
		scene->game->context->quit = true;

	if(Context_GetKey(scene->game->context, INPUT_UP))
		dy--;

	if(Context_GetKey(scene->game->context, INPUT_DOWN))
		dy++;

	if(Context_GetKey(scene->game->context, INPUT_LEFT))
		dx--;

	if(Context_GetKey(scene->game->context, INPUT_RIGHT))
		dx++;

	if(entity->state == SELECTOR_STATE_LOST){
		entity->next_think = scene->tick + 150;

		if(b_open_tile)
			scene->loadNextScene = load_next_scene;
	}
	else if(dx == 0 && dy == 0){
		entity->state = SELECTOR_STATE_NULL;
		entity->next_think = scene->tick + 16;
	}
	else if(entity->state == SELECTOR_STATE_NULL){
		x += dx;
		y += dy;
		entity->state = SELECTOR_STATE_MOVING;
		entity->next_think = scene->tick + 150;
	}
	else if(entity->state == SELECTOR_STATE_MOVING){
		x += dx;
		y += dy;
		entity->state = SELECTOR_STATE_MOVING_ON;
		entity->next_think = scene->tick + 150;
	}
	else if(entity->state == SELECTOR_STATE_MOVING_ON){
		x += dx;
		y += dy;
		entity->state = SELECTOR_STATE_MOVING_ON;
		entity->next_think = scene->tick + 150;
	}

	if(x < 0) x = 0;
	if(x >= WORLD_WIDTH) x = WORLD_WIDTH - 1;
	if(y < 0) y = 0;
	if(y >= WORLD_HEIGHT) y = WORLD_HEIGHT - 1;

	entity->position.x = WORLD_TILE_WIDTH * x;
	entity->position.y = WORLD_TILE_WIDTH * y;

	if(b_open_tile){
		if(Scene_GetTileId(scene, x, y, WORLD_LAYER_BACKGROUND) == -1)
			build_world(scene, x, y);

		if(open_tile(scene, x, y)){
			if(entity->state == SELECTOR_STATE_NULL)
				entity->next_think = scene->tick + 50;

			entity->state = SELECTOR_STATE_LOST;
		}

		if(check_win_state(scene)){
			if(entity->state == SELECTOR_STATE_NULL)
				entity->next_think = scene->tick + 100;

			entity->state = SELECTOR_STATE_LOST;

		}
	}

	if(b_put_flag){
		int id = Scene_GetTileId(scene, x, y, WORLD_LAYER_FOREGROUND);

		if(id == TILE_FLAG){
			Scene_SetTileId(scene, x, y, WORLD_LAYER_FOREGROUND, TILE_NONE);
			num_flag--;
		}
		else if(id != -1){
			Scene_SetTileId(scene, x, y, WORLD_LAYER_FOREGROUND, TILE_FLAG);
			num_flag++;
		}

		entity->next_think = scene->tick + 150;
	}
}

int bomb_count(Scene *scene, int x, int y){
	int count = 0;

	for(int i = x - 1; i <= x + 1; i++){
		for(int j = y - 1; j <= y + 1; j++){
			if(Scene_GetTileId(scene, i, j, WORLD_LAYER_BACKGROUND) == TILE_BOMB)
				count++;
		}
	}

	return count;
}


int flag_count(Scene *scene, int x, int y){
	int count = 0;

	for(int i = x - 1; i <= x + 1; i++){
		for(int j = y - 1; j <= y + 1; j++){
			if(i == x && j == y)
				continue;

			if(Scene_GetTileId(scene, i, j, WORLD_LAYER_FOREGROUND) == TILE_FLAG)
				count++;
		}
	}

	return count;
}

void open_rec(Scene *scene, int x, int y){
	for(int i = x - 1; i <= x + 1; i++){
		for(int j = y - 1; j <= y + 1; j++){
			if(i == x && j == y)
				continue;

			int id = Scene_GetTileId(scene, i, j, WORLD_LAYER_FOREGROUND);

			if(id == TILE_FLAG)
				continue;

			open_tile(scene, i, j);
		}
	}
}

bool open_flag(Scene *scene, int x, int y){
	bool found = false;

	for(int i = x - 1; i <= x + 1; i++){
		for(int j = y - 1; j <= y + 1; j++){
			if(i == x && j == y)
				continue;

			int id = Scene_GetTileId(scene, i, j, WORLD_LAYER_FOREGROUND);

			if(id == TILE_FLAG)
				continue;

			if(Scene_GetTileId(scene, i, j, WORLD_LAYER_BACKGROUND) == 0){
				open_tile(scene, i, j);
			}

			Scene_SetTileId(scene, i, j, WORLD_LAYER_FOREGROUND, -1);

			if(Scene_GetTileId(scene, i, j, WORLD_LAYER_BACKGROUND) == TILE_BOMB)
				found = true;
		}
	}

	return found;
}

bool open_tile(Scene *scene, int x, int y){
	if(x < 0 || y < 0 || x >= WORLD_WIDTH || y >= WORLD_HEIGHT)
		return false;

	int id_fg = Scene_GetTileId(scene, x, y, WORLD_LAYER_FOREGROUND);
	int id_bg = Scene_GetTileId(scene, x, y, WORLD_LAYER_BACKGROUND);

	if(id_fg == TILE_FLAG)
		return false;

	Scene_SetTileId(scene, x, y, WORLD_LAYER_FOREGROUND, -1);

	if(id_bg == TILE_BOMB)
		return true;

	if(id_fg == -1){
		if(id_bg == flag_count(scene, x, y) && id_bg != 0){
			return open_flag(scene, x, y);
		}
		else{
			return false;
		}
	}

	if(id_bg == 0){
		open_rec(scene, x, y);
	}

	return false;
}

void build_world(Scene *scene, int x, int y){
	for(size_t i = 0; i < WORLD_WIDTH; i++){
		for(size_t j = 0; j < WORLD_HEIGHT; j++){
			Scene_SetTileId(scene, i, j, WORLD_LAYER_BACKGROUND, 0);
		}
	}

	for(size_t i = 0; i < NUM_BOMBS; i++){
		int pos_x = rand() % WORLD_WIDTH;
		int pos_y = rand() % WORLD_HEIGHT;

		if(Scene_GetTileId(scene, pos_x, pos_y, WORLD_LAYER_BACKGROUND) == TILE_BOMB || (abs(pos_x - x) <= 1 && abs(pos_y - y) <= 1)){
			i--;
			continue;
		}

		Scene_SetTileId(scene, pos_x, pos_y, WORLD_LAYER_BACKGROUND, TILE_BOMB);
	}

	for(size_t i = 0; i < WORLD_WIDTH; i++){
		for(size_t j = 0; j < WORLD_HEIGHT; j++){
			Scene_SetTileId(scene, i, j, WORLD_LAYER_FOREGROUND, TILE_NONE);

			if(Scene_GetTileId(scene, i, j, WORLD_LAYER_BACKGROUND) == TILE_BOMB)
				continue;

			Scene_SetTileId(scene, i, j, WORLD_LAYER_BACKGROUND, bomb_count(scene, i, j));
		}
	}
}

int main(int argc, char **argv){
	srand(time(NULL));
	Context *context;
	Game *game;
	Mems memory;

	(void) argc;
	(void) argv;

	/* Criar a estrutura que organizará a memória */
	memory = Mems_Create(malloc(MEMORY_USED), MEMORY_USED);

	context = Context_Create("Campo Minado PSP", INTERNAL_WIDTH, INTERNAL_HEIGHT, &memory);
	game = Game_Create(context);

	MegaTexture_Load(&game->resources->mega_textures[0], context, "tile.png");

	Texture_Create(
			&game->resources->textures[0], 
			&game->resources->mega_textures[0],
			0,
			0,
			224,
			16,
			WORLD_TILE_WIDTH,
			WORLD_TILE_HEIGHT
			);

	Texture_Create(
			&game->resources->textures[1], 
			&game->resources->mega_textures[0],
			0,
			16,
			128,
			24,
			HUD_TILE_WIDTH,
			HUD_TILE_HEIGHT
			);

	load_next_scene(game->main_scene);

	Context_SetFps(context, 60);

	Game_Run(game);

	MegaTexture_Destroy(&game->resources->mega_textures[0]);
	Context_Destroy(context);
	free(memory.block);

	printf("%lu\n", memory.top);

	return 0;
}
