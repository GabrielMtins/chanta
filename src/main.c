#include <stdio.h>
#include "context.h"
#include "game.h"

int main(int argc, char **argv){
	Context context;
	Game game;

	(void) argc;
	(void) argv;

	context = Context_Create("hi", 480, 272);
	game = Game_Create(&context);

	Game_Run(&game);

	Context_Destroy(&context);

	return 0;
}
