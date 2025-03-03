#include "scene.h"
#include "box.h"

static uint8_t Scene_GetTileId(Scene *scene, int x, int y, int layer);
static bool Scene_SetTileId(Scene *scene, int x, int y, int layer, uint8_t id);
static bool Scene_CheckCollisionWorld(Scene *scene, Entity *entity);
static bool Scene_HandlePhysics(Scene *scene);
static bool Scene_HandleEntityCollision(Scene *scene);
static bool Scene_RenderWorld(Scene *scene, int layer);
static bool Scene_Render(Scene *scene);

bool Scene_Reset(Scene *scene, Game *game){
	scene->game = game;
	scene->top_free_index = -1;
	scene->num_entities = 0;

	memset(scene->world->tiles, 0, WORLD_DATA_SIZE);

	for(size_t i = 0; i < MAX_ENTITIES; i++){
		scene->entities[i].removed = true;
		scene->entities[i].free = false;
	}

	Scene_SetTileId(scene, 8, 8, WORLD_LAYER_FOREGROUND, 1);
	Scene_SetTileId(scene, 8, 9, WORLD_LAYER_FOREGROUND, 1);
	Scene_SetTileId(scene, 9, 8, WORLD_LAYER_FOREGROUND, 1);
	Scene_SetTileId(scene, 9, 9, WORLD_LAYER_FOREGROUND, 1);

	scene->world->collision_layer = 1;

	return true;
}

bool Scene_Update(Scene *scene){
	if(scene == NULL)
		return false;

	Scene_HandlePhysics(scene);
	Scene_HandleEntityCollision(scene);
	Scene_Render(scene);

	return true;
}

bool Scene_SetWorldTexture(Scene *scene, Texture *texture){
	scene->world->texture = texture;

	return true;
}

Entity * Scene_AddEntity(Scene *scene){
	if(scene == NULL)
		return NULL;

	if(scene->top_free_index < 0){
		if(scene->num_entities == MAX_ENTITIES)
			return NULL;
		else
			return &scene->entities[scene->num_entities++];
	}

	return scene->free_entities_stack[scene->top_free_index--];
}

bool Scene_RemoveEntity(Scene *scene, Entity *entity){
	if(scene == NULL)
		return false;

	scene->free_entities_stack[++scene->top_free_index] = entity;
	entity->removed = true;
	entity->free = false;

	return true;
}

static uint8_t Scene_GetTileId(Scene *scene, int x, int y, int layer){
	int index;

	if(x < 0 || y < 0 || x >= WORLD_WIDTH || y >= WORLD_HEIGHT)
		return WORLD_TILE_OUT_OF_BOUNDS;

	if(layer < 0 || layer >= WORLD_NUM_LAYERS)
		return WORLD_TILE_OUT_OF_BOUNDS;

	index = layer * WORLD_WIDTH * WORLD_HEIGHT + WORLD_WIDTH * y + x;

	return scene->world->tiles[index];
}

static bool Scene_SetTileId(Scene *scene, int x, int y, int layer, uint8_t id){
	int index;

	if(x < 0 || y < 0 || x >= WORLD_WIDTH || y >= WORLD_HEIGHT)
		return false;

	if(layer < 0 || layer >= WORLD_NUM_LAYERS)
		return false;

	index = layer * WORLD_WIDTH * WORLD_HEIGHT + WORLD_WIDTH * y + x;

	scene->world->tiles[index] = id;

	return true;
}

static bool Scene_CheckCollisionWorld(Scene *scene, Entity *entity){
	int start_x, start_y, end_x, end_y;

	if(scene == NULL || entity->removed || entity == NULL)
		return false;

	if((scene->world->collision_layer & entity->collision_mask) == 0 && (scene->world->collision_layer & entity->trigger_mask) == 0)
		return false;

	start_x = floorf(entity->position.x / WORLD_TILE_WIDTH);
	start_y = floorf(entity->position.y / WORLD_TILE_HEIGHT);
	end_x = ceilf((entity->position.x + entity->size.x) / WORLD_TILE_WIDTH);
	end_y = ceilf((entity->position.y + entity->size.y) / WORLD_TILE_HEIGHT);

	for(int i = start_x; i < end_x; i++){
		for(int j = start_y; j < end_y; j++){
			if(Scene_GetTileId(scene, i, j, WORLD_LAYER_FOREGROUND) != 0)
				return true;
		}
	}

	return false;
}

static bool Scene_HandlePhysics(Scene *scene){
	Entity *current;
	Vec2 delta;
	bool has_collision, found_collision;

	for(size_t i = 0; i < scene->num_entities; i++){
		found_collision = false;
		current = &scene->entities[i];

		if(current->removed)
			continue;

		Vec2_Mul(&delta, &current->velocity, scene->game->context->dt);

		current->position.x += delta.x;

		has_collision = (current->collision_mask & scene->world->collision_layer) != 0;

		if(Scene_CheckCollisionWorld(scene, current)){
			if(has_collision){
				if(current->velocity.x > 0){
					current->position.x = floorf((current->position.x + current->size.x) / WORLD_TILE_WIDTH) * WORLD_TILE_WIDTH - current->size.x;
				}
				else{
					current->position.x = ceilf(current->position.x / WORLD_TILE_WIDTH) * WORLD_TILE_WIDTH;
				}
			}

			found_collision = true;
		}

		current->position.y += delta.y;

		if(Scene_CheckCollisionWorld(scene, current)){
			if(has_collision){
				if(current->velocity.y > 0){
					current->position.y = floorf((current->position.y + current->size.y) / WORLD_TILE_HEIGHT) * WORLD_TILE_HEIGHT - current->size.y;
				}
				else{
					current->position.y = ceilf(current->position.y / WORLD_TILE_HEIGHT) * WORLD_TILE_HEIGHT;
				}
			}

			found_collision = true;
		}

		if(found_collision){
			if(has_collision && current->onCollision != NULL)
				current->onCollision(scene, current, NULL);
			else if(current->onTrigger != NULL)
				current->onTrigger(scene, current, NULL);
		}
	}

	return true;
}

static bool Scene_HandleEntityCollision(Scene *scene){
	Entity *current;
	Entity *other;
	Vec2 old_pos;

	for(size_t i = 0; i < scene->num_entities; i++){
		current = &scene->entities[i];

		if(current->removed)
			continue;

		for(size_t j = 0; j < scene->num_entities; j++){
			other = &scene->entities[j];

			bool is_mask = (current->collision_mask & other->collision_layer) != 0;
			bool is_trigger = (current->trigger_mask & other->collision_layer) != 0;

			if(i == j || other->removed || !(is_mask || is_trigger))
				continue;

			old_pos = current->position;

			if(is_trigger && Box_CheckCollision(&current->position, &current->size, &other->position, &other->size)){
				if(current->onTrigger != NULL)
					current->onTrigger(scene, current, other);
			}
			else if(is_mask && Box_SolveCollision(&current->position, &current->size, &other->position, &other->size)){
				bool has_collision_world = (current->collision_mask & scene->world->collision_layer) != 0;

				if(Scene_CheckCollisionWorld(scene, current) && has_collision_world){
					current->position = old_pos;
				}

				if(current->onCollision != NULL)
					current->onCollision(scene, current, other);
			}
		}
	}

	return true;
}

static bool Scene_RenderWorld(Scene *scene, int layer){
	int start_x, start_y, end_x, end_y;
	int screen_x, screen_y;

	if(layer < 0 || layer >= WORLD_NUM_LAYERS)
		return false;

	start_x = floorf(scene->camera.x / WORLD_TILE_WIDTH);
	start_y = floorf(scene->camera.y / WORLD_TILE_HEIGHT);
	end_x = ceilf((scene->camera.y + scene->game->context->width) / WORLD_TILE_WIDTH);
	end_y = ceilf((scene->camera.y + scene->game->context->height) / WORLD_TILE_HEIGHT);

	for(int i = start_x; i < end_x; i++){
		for(int j = start_y; j < end_y; j++){
			uint8_t id = Scene_GetTileId(scene, i, j, layer);
			if(id == 0)
				continue;
			id--;

			screen_x = i * WORLD_TILE_WIDTH - (int) scene->camera.x;
			screen_y = j * WORLD_TILE_WIDTH - (int) scene->camera.y;

			Texture_Render(
					scene->game->context,
					scene->world->texture,
					screen_x,
					screen_y,
					id,
					0
					);
		}
	}

	return true;
}

static bool Scene_Render(Scene *scene){
	Entity *current;

	Scene_RenderWorld(scene, WORLD_LAYER_BACKGROUND);
	Scene_RenderWorld(scene, WORLD_LAYER_FOREGROUND);

	for(size_t i = 0; i < scene->num_entities; i++){
		current = &scene->entities[i];

		if(current->removed || current->texture == NULL)
			continue;

		Texture_Render(
				scene->game->context,
				current->texture,
				current->position.x - scene->camera.x,
				current->position.y - scene->camera.y,
				current->cell_id,
				0
				);
	}

	Scene_RenderWorld(scene, WORLD_LAYER_DETAIL);

	return true;
}
