#include "entity.h"

bool Entity_Reset(Entity *ent){
	if(ent == NULL)
		return false;

	ent->position = (Vec2){0.0f, 0.0f};
	ent->size     = (Vec2){0.0f, 0.0f};
	ent->velocity = (Vec2){0.0f, 0.0f};

	ent->collision_layer = 0;
	ent->collision_mask  = 0;

	ent->free = true;
	ent->think = NULL;

	return true;
}
