#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "textures.h"

SDL_Texture *load_texture(char *filename, SDL_Renderer *renderer) {
    SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
    if (texture == NULL) {
        fprintf(stderr, "failed to load texture: '%s'\n", filename);
    }
    return texture;
}
