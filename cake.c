#include "cake.h"
#include "textures.h"
#include "constants.h"

struct Cake cake_create(SDL_Renderer *renderer) {
    struct Cake cake = {
        .rect = {
            .x = 10,
            .y = 10,
            .w = 64,
            .h = 64
        },
        .state = 0,
        .texture_rect = {
            .x = 0,
            .y = 0,
            .w = 64,
            .h = 64
        },
        .center = {
            .x = 64,
            .y = 64
        },
    };

    char buf[255];
    for (int i = 0; i < 13; i++) {
        sprintf(buf, "assets/cake_eaten%d.png", i);
        cake.textures[i] = load_texture(buf, renderer);
    }
    return cake;
}

void cake_render(SDL_Renderer *renderer, struct Cake *cake) {
    SDL_RenderCopyEx(
        renderer,
        cake->textures[cake->state],
        &cake->texture_rect,
        &cake->rect,
        0.0, // angle
        &cake->center, // center
        SDL_FLIP_NONE
    );
}

void cake_eat(struct Cake *cake) {
    cake->state = (cake->state + 1) % 13;
}
