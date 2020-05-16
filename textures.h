#ifndef TEXTURES_H
#define TEXTURES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Texture *load_texture(char *filename, SDL_Renderer *renderer);

#endif /* TEXTURES_H */
