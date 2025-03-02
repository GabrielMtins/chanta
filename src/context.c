#include "context.h"

Context Context_Create(const char *title, int w, int h){
	Context context;

	context.window = SDL_CreateWindow(
			title,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			w, h,
			0
			);

	context.renderer = SDL_CreateRenderer(
			context.window,
			-1,
			SDL_RENDERER_PRESENTVSYNC
			);

	context.quit = false;
	context.tick = 0;

	return context;
}

void Context_PollEvent(Context *context){
	SDL_Event e;
	size_t new_tick;

	new_tick = SDL_GetTicks();
	context->dt = 0.001f * (new_tick - context->tick);
	context->tick = new_tick;
	
	while(SDL_PollEvent(&e)){
		if(e.type == SDL_QUIT)
			context->quit = true;
	}
}

bool Context_Destroy(Context *context){
	SDL_DestroyRenderer(context->renderer);
	SDL_DestroyWindow(context->window);

	return true;
}
