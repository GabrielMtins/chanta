#include "texture.h"

bool MegaTexture_Load(MegaTexture *mega_texture, Context *context, const char *filename){
	SDL_Surface *surface;

	if(mega_texture == NULL)
		return false;

	surface = IMG_Load(filename);

	if(surface == NULL){
		mega_texture->texture = NULL;
		return false;
	}

	mega_texture->texture = SDL_CreateTextureFromSurface(context->renderer, surface);
	SDL_FreeSurface(surface);

	return true;
}

bool MegaTexture_Destroy(MegaTexture *mega_texture){
	if(mega_texture == NULL)
		return false;

	if(mega_texture->texture == NULL)
		return false;

	SDL_DestroyTexture(mega_texture->texture);

	return true;
}

bool Texture_Create(Texture *texture, MegaTexture *mega_texture, int x, int y, int w, int h, int cell_w, int cell_h){
	if(mega_texture == NULL || texture == NULL)
		return false;

	(void) h;

	texture->mega_texture = mega_texture;

	texture->x = x;
	texture->y = y;
	texture->cell_w = cell_w;
	texture->cell_h = cell_h;

	texture->num_cells_x = w / cell_w;

	return true;
}

bool Texture_Render(Context *context, Texture *texture, int x, int y, int id, int flip){
	SDL_Rect src_rect, dst_rect;

	src_rect = (SDL_Rect) {
		texture->x + (id % texture->num_cells_x) * texture->cell_w,
		texture->y + (id / texture->num_cells_x) * texture->cell_h,
		texture->cell_w,
		texture->cell_h
	};

	dst_rect = (SDL_Rect) {x, y, texture->cell_w, texture->cell_h};

	SDL_RenderCopyEx(
			context->renderer,
			texture->mega_texture->texture,
			&src_rect,
			&dst_rect,
			0,
			NULL,
			flip
			);

	return true;
}
