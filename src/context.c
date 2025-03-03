#include "context.h"

Context *Context_Create(const char *title, int w, int h, Mems *memory){
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	Mix_Init(MIX_INIT_MP3);

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
		return NULL;
	}

	Context *context;

	context = (Context *) Mems_Alloc(memory, sizeof(Context));

	context->window = SDL_CreateWindow(
			title,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			2 * w, 2 * h,
			SDL_WINDOW_RESIZABLE
			);

	context->renderer = SDL_CreateRenderer(
			context->window,
			-1,
			SDL_RENDERER_PRESENTVSYNC
			);

	context->width = w;
	context->height = h;

	SDL_RenderSetLogicalSize(context->renderer, w, h);

	context->memory = memory;

	context->quit = false;
	context->tick = 0;
	context->data = NULL;
	context->dt = 0.0f;

	return context;
}

bool Context_SetDataFromMem(Context *context, const char *data, int size){
	context->data = SDL_RWFromConstMem((const void *) data, size);

	return context->data != NULL;
}

bool Context_SetDataFromFile(Context *context, const char *filename){
	context->data = SDL_RWFromFile(filename, "rb");

	return context->data != NULL;
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

	IMG_Quit();
	SDL_Quit();

	return true;
}
