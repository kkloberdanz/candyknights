#ifndef CAKE_H
#define CAKE_H

#include <SDL2/SDL.h>

struct Cake {
    int state;
    SDL_Rect rect;
    SDL_Rect texture_rect;
    SDL_Point center;
    SDL_Texture *textures[13];
};

struct Cake cake_create(SDL_Renderer *renderer);

void cake_render(SDL_Renderer *renderer, struct Cake *cake);

void cake_eat(struct Cake *cake);

#endif /* CAKE_H */
