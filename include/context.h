#ifndef CONTEXT_H
#define CONTEXT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "mems.h"

typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_RWops *data;

	int width, height;

	size_t tick;
	size_t delta_tick;
	float dt;

	size_t min_time_frame;

	bool quit;

	Mems *memory;
} Context;

Context *Context_Create(const char *title, int w, int h, Mems *memory);

bool Context_SetFps(Context *context, size_t fps);

bool Context_SetDataFromMem(Context *context, const char *data, int size);

bool Context_SetDataFromFile(Context *context, const char *filename);

void Context_PollEvent(Context *context);

bool Context_Destroy(Context *context);

#endif
