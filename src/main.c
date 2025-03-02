#include <stdio.h>
#include "context.h"
#include "game.h"
#include "mems.h"
#include "scene.h"

#define MEMORY_USED 1024 * 1024 * 16

int main(int argc, char **argv){
	Context context;
	Game game;
	Scene *scene;
	Mems mems;

	mems = Mems_Create(malloc(MEMORY_USED), MEMORY_USED);

	(void) argc;
	(void) argv;

	scene = Scene_Setup(&mems);

	context = Context_Create("hi", 640, 480);
	game = Game_Create(&context, scene, Mems_Alloc(&mems, sizeof(Resource)));

	Game_Run(&game);

	Context_Destroy(&context);

	free(mems.block);

	return 0;
}
