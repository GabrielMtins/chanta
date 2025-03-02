#include "scene.h"

static bool Scene_HandleCollision(Scene *scene);

Scene *Scene_Setup(Mems *mems){
	Scene *scene;

	scene = Mems_Alloc(mems, sizeof(Scene));
	scene->entities = Mems_Alloc(mems, sizeof(Entity) * MAX_ENTITIES);

	return scene;
}

bool Scene_Reset(Scene *scene, Game *game){
	scene->game = game;
	scene->top_free_index = -1;
	scene->num_entities = 0;

	return true;
}

bool Scene_Update(Scene *scene){
	if(scene == NULL)
		return NULL;

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

static bool Scene_HandleCollision(Scene *scene){
	Entity *current;

	for(size_t i = 0; i < scene->num_entities; i++){
		current = &scene->entities[i];

		if(current->removed)
			continue;

		current->
	}

	return true;
}
