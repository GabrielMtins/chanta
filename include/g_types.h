#ifndef G_TYPES_H
#define G_TYPES_H

#define MAX_ENTITIES 256

#include "context.h"
#include "vec2.h"
#include "texture.h"
#include "resource.h"

typedef struct scene_s Scene;
typedef struct game_s Game;
typedef struct entity_s Entity;

struct game_s {
	Context *context;
	Scene *main_scene;
	Resource *resources;
};

struct scene_s {
	Game *game;

	/* array de entidades */
	Entity *entities;
	size_t num_entities;

	/* stack de memória que indica quais entidades estão livres */
	Entity *free_entities_stack[MAX_ENTITIES];
	int top_free_index;
};

/* Variáveis customizáveis da entidade. */
typedef struct {
	char tmp;
} EntVars;

/* Estrutura da entidade de fato. Se quiser adicionar componentes
 * a mais, adicione na EntVars por questão de organização */
struct entity_s {
	/* Essa flag é interna utilizada para saber se a entidade é válida ou se foi removida. */
	bool removed;
	/* Marque essa flag como true para que seja liberada da memória no próximo frame. */
	bool free;

	/* Tipo da entidade. */
	int type;

	Vec2 position;
	Vec2 size;
	Vec2 velocity;

	/* Máscaras de colisão. */
	uint32_t collision_layer;
	uint32_t collision_mask;
	uint32_t trigger_mask;

	/* Pointer para indicar um target: inimigo etc */
	Entity *target;
	/* Pointer para indicar um filho da entidade */
	Entity *child;

	/* Flag para máquina de estado */
	int state;

	/* Textura atual utilizada */
	Texture *texture;
	/* Id da célula da texture */
	int cell_id;

	/* Tempo em ms para o próximo tempo em que a função think() será chamada */
	size_t next_think;

	void (*update)(Scene *, Entity *, float);
	void (*think)(Scene *, Entity *);
	void (*onCollision)(Scene *, Entity *, Entity *);
	void (*onTrigger)(Scene *, Entity *, Entity *);

	/* Variáveis customizadas para entidade */
	EntVars v;
};

#endif
