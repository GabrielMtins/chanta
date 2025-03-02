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

typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;

	size_t tick;
	float dt;

	bool quit;
} Context;

Context Context_Create(const char *title, int w, int h);

void Context_PollEvent(Context *context);

bool Context_Destroy(Context *context);

#endif
