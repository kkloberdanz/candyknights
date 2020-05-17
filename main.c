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
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "entity.h"
#include "sprite.h"
#include "textures.h"
#include "constants.h"

int rand_ball_velocity() {
    int random_num = rand() % MAX_VELOCITY;
    int velocity = MAX(random_num, MIN_VELOCITY);
    return velocity;
}

bool obj_in_bounds(SDL_Rect *rect) {
    if (rect->x > SCREEN_WIDTH) {
        return false;
    } else if (rect->x < 0) {
        return false;
    } else if (rect->y > SCREEN_HEIGHT) {
        return false;
    } else if (rect->y < 0) {
        return false;
    } else {
        return true;
    }
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

int entity_render_compare(const void *a, const void *b) {
    struct Entity *e1 = *(struct Entity **)a;
    struct Entity *e2 = *(struct Entity **)b;
    return e1->rect.y - e2->rect.y;
}

char game_loop(
    SDL_Joystick *joystick,
    SDL_Renderer *renderer
) {
    SDL_Texture *background_image = load_texture(
        "assets/background.png",
        renderer
    );

    struct Entity player = create_knight(renderer);
    struct Entity enemy = create_knight(renderer);

    /* make enemies slightly slower than player */
    enemy.y_vel = 0.9 * player.y_vel;
    enemy.x_vel = 0.9 * player.x_vel;

    player.dir = LEFT;
    player.state = WALKING;
    player.handicap = 0;
    player.max_handicap = 0;
    entity_logic(&player, NULL, 0); // setup player position

    struct Entity *entities[2];
    entities[0] = &player;
    entities[1] = &enemy;
    const size_t num_entities = sizeof(entities) / sizeof(struct Entity *);
    entity_set_pos(&player, 0.8*SCREEN_WIDTH, player.rect.y);
    entity_set_pos(&enemy, 0.2*SCREEN_WIDTH, player.rect.y);

    while (game_running()) {
        int start_tick = SDL_GetTicks();

        if (handle_player_input(&player, joystick) == END_GAME) {
            goto cleanup;
        }

        entity_logic(&player, entities, num_entities);

        enemy_ai_logic(&player, &enemy);
        entity_logic(&enemy, entities, num_entities);

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background_image, NULL, NULL);

        qsort(
            entities,
            num_entities,
            sizeof(struct Entity *),
            entity_render_compare
        );

        for (size_t i = 0; i < num_entities; i++) {
            entity_render(entities[i], renderer);
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(20 - (start_tick - SDL_GetTicks()));
    }

cleanup:
    SDL_DestroyTexture(background_image);
    return 0;
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
        exit(EXIT_FAILURE);
    }

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

    SDL_ShowCursor(SDL_DISABLE);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    int status_code = game_loop(joystick, renderer);

    puts("destroy window");
    SDL_DestroyWindow(window);
    puts("shutting down sdl");
    SDL_JoystickClose(joystick);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    puts("done");
    return status_code;
}
