/*
 * Author: Kyle Kloberdanz
 * Project Start Date: 13 July 2019
 * License: GNU GPLv3 (see LICENSE.txt)
 *     This file is part of candymatch.
 *
 *     candymatch is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 *
 *     candymatch is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License
 *     along with candymatch.  If not, see <https://www.gnu.org/licenses/>.
 * File: candymatch.c
 */

#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define MAX(A, B) (((A) > (B)) ? (A) : (B))

enum {
    SCREEN_WIDTH = 800,
    SCREEN_HEIGHT = 600,
};

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

char *direction_str[] = {
    "UP",
    "DOWN",
    "LEFT",
    "RIGHT"
};

enum Button {
    B = 0,
    A = 1,
    START = 9,
    SELECT = 8
};

struct Entity {
    SDL_Rect rect;
    SDL_Texture *texture;
};

char *get_button_str(enum Button button) {
    switch (button) {
        case B:
            return "B";
        case A:
            return "A";
        case START:
            return "START";
        case SELECT:
            return "SELECT";
        default:
            return "NOT A BUTTON";
    }
}

int get_button(SDL_Joystick *joystick) {
    int num_buttons = SDL_JoystickNumButtons(joystick);
    for (int i = 0; i < num_buttons; i++) {
        if (SDL_JoystickGetButton(joystick, i)) {
            return i;
        }
    }
    return -1;
}

int get_direction(SDL_Joystick *joystick) {
    int num_axes = SDL_JoystickNumAxes(joystick);
    for (int i = 0; i < num_axes; i++) {
        int axis = SDL_JoystickGetAxis(joystick, i);
        if (axis) {
            if (i == 1) {
                if (axis < 0) {
                    return UP;
                } else {
                    return DOWN;
                }
            } else if (i == 0) {
                if (axis < 0) {
                    return LEFT;
                } else {
                    return RIGHT;
                }
            }
        }
    }
    return -1;
}

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
    /* initialize */
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

    SDL_Surface *screen_surface = SDL_GetWindowSurface(window);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_Texture *background_image = load_texture("assets/background.png", screen_surface, renderer);

    struct Entity cake = {
        .rect = {
            .x = 0,
            .y = 0,
            .w = 32,
            .h = 32
        },
        .texture = load_texture("assets/cake.png", screen_surface, renderer)
    };

    int y_vel = 10;
    int x_vel = 10;
    int size_factor = 1;
    while (game_running()) {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background_image, NULL, NULL);
        SDL_RenderCopy(renderer, cake.texture, NULL, &cake.rect);
        SDL_RenderPresent(renderer);
        cake.rect.x += x_vel;
        if (cake.rect.x > SCREEN_WIDTH || cake.rect.x <= 0) {
            x_vel = x_vel * -1;
        }
        cake.rect.y += y_vel;
        if (cake.rect.y > SCREEN_HEIGHT || cake.rect.y <= 0) {
            y_vel = y_vel * -1;
        }

        cake.rect.w += size_factor;
        cake.rect.h += size_factor;

        if (cake.rect.w > SCREEN_WIDTH) {
            size_factor = -1;
        } else if (cake.rect.w < 10) {
            size_factor = 1;
        }

        SDL_Delay(20);
    }

#if 0

    int num_joysticks = SDL_NumJoysticks();
    printf("found %d joysticks\n", num_joysticks);
    for (int i = 0; i < num_joysticks; ++i) {
        if (SDL_IsGameController(i)) {
            printf("Joystick %i is supported!\n", i);
        }
    }

    SDL_Joystick *joystick = NULL;
    if (num_joysticks) {
        joystick = SDL_JoystickOpen(0);
    } else {
        goto quit;
    }

    int num_buttons = SDL_JoystickNumButtons(joystick);
    int num_hats = SDL_JoystickNumHats(joystick);
    printf("num buttons: %d\n", num_buttons);
    printf("num hats: %d\n", num_hats);
    printf("num axis: %d\n", SDL_JoystickNumAxes(joystick));
    printf("num balls: %d\n", SDL_JoystickNumBalls(joystick));

    while (game_running()) {
        int button = get_button(joystick);
        if (button >= 0) {
            printf("button: %s\n", get_button_str(button));
        }

        int dir = get_direction(joystick);
        if (dir >= 0) {
            printf("direction: %s\n", direction_str[dir]);
        }

        SDL_Flip(backbuffer);
        SDL_Delay(20);
    }

    SDL_JoystickClose(joystick);
quit:
#endif
    puts("destroy window");
    SDL_DestroyWindow(window);
    puts("shutting down sdl");
    SDL_Quit();
    puts("done");
    return 0;
}
