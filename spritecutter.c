#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

enum {
    SCREEN_WIDTH = 800,
    SCREEN_HEIGHT = 600,
};

SDL_Surface *load_image(char *filename, SDL_Surface *screen) {
    SDL_Surface *surface = IMG_Load(filename);
    SDL_Surface *optimized_surface = NULL;

    if (surface == NULL) {
        fprintf(stderr, "unable to load image: %s\n", filename);
    } else {
        int color_key = SDL_MapRGB(surface->format, 0x00, 0x00, 0x00);
        SDL_SetColorKey(surface, SDL_TRUE, color_key);
        optimized_surface = SDL_ConvertSurface(surface, screen->format, 0);
        SDL_FreeSurface(surface);
    }
    return optimized_surface;
}

SDL_Texture *load_texture(
    char *filename,
    SDL_Surface *screen,
    SDL_Renderer *renderer
) {
    SDL_Surface *image = load_image(filename, screen);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    return texture;
}

bool game_running() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return false;

            default:
                break;
        }
    }
    return true;
}

int main(void) {
    SDL_Init(SDL_INIT_EVERYTHING);
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0) {
        fprintf(stderr, "failed to initialize image library\n");
        exit(EXIT_FAILURE);
    };
    SDL_Renderer *renderer;
    SDL_Window *window;

    /* create window and renderer */
    SDL_CreateWindowAndRenderer(
        0,
        0,
        SDL_WINDOW_FULLSCREEN,
        &window,
        &renderer
    );

    if (SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT) < 0) {
        fprintf(stderr, "failed to set resolution: %s\n", SDL_GetError());
    }

    SDL_Surface *screen = SDL_GetWindowSurface(window);
    SDL_Texture *knight_texture = load_texture("assets/knight.png", screen, renderer);

    SDL_Rect knight_rect = {
        .h = 64,
        .w = 64,
        .x = 0,
        .y = 0
    };

    for (int i = 0; i < 10; i++) {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, knight_texture, &knight_rect, NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000);
        knight_rect.x += 64;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
