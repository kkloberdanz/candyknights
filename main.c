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

#define MAX(A, B) (((A) > (B)) ? (A) : (B))

enum Constants {
    SCREEN_WIDTH = 800,
    SCREEN_HEIGHT = 600,
    MAX_VELOCITY = 20,
    MIN_VELOCITY = 10,
    WINNING_SCORE = 10
};

enum Direction {
    UP = 1,
    DOWN = 2,
    LEFT = 4,
    RIGHT = 8
};

enum ControllerButton {
    CTRL_A = 1,
    CTRL_B = 0,
    CTRL_START = 9,
    CTRL_SELECT = 8
};

enum Button {
    A = 1,
    B = 2,
    START = 4,
    SELECT = 8
};

int rand_ball_velocity() {
    int random_num = rand() % MAX_VELOCITY;
    int velocity = MAX(random_num, MIN_VELOCITY);
    return velocity;
}

bool obj_touching(SDL_Rect *rect1, SDL_Rect *rect2) {
    return !(rect1->x >= rect2->x + rect2->w) &&
           !(rect1->y >= rect2->y + rect2->h) &&
           !(rect2->x >= rect1->x + rect1->w) &&
           !(rect2->y >= rect1->y + rect1->h);
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

int translate_controller_button(int button_num) {
    switch (button_num) {
        case CTRL_A:
            return A;
        case CTRL_B:
            return B;
        case CTRL_START:
            return START;
        case CTRL_SELECT:
            return SELECT;
        default:
            return -1;
    }
}

char get_button(SDL_Joystick *joystick) {
    int num_buttons = SDL_JoystickNumButtons(joystick);
    char button = 0;
    for (int i = 0; i < num_buttons; i++) {
        if (SDL_JoystickGetButton(joystick, i)) {
            int button_pressed = translate_controller_button(i);
            if (button_pressed != -1) {
                button |= button_pressed;
            }
        }
    }
    return button;
}

char get_direction(SDL_Joystick *joystick) {
    int num_axes = SDL_JoystickNumAxes(joystick);
    char direction = 0;
    for (int i = 0; i < num_axes; i++) {
        int axis = SDL_JoystickGetAxis(joystick, i);
        if (axis) {
            if (i == 1) {
                if (axis < 0) {
                    direction |= UP;
                } else {
                    direction |= DOWN;
                }
            } else if (i == 0) {
                if (axis < 0) {
                    direction |= LEFT;
                } else {
                    direction |= RIGHT;
                }
            }
        }
    }
    return direction;
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

void player_logic(struct Entity *entity, enum Direction dir) {
    switch (entity->state) {
        case ATTACKING:
            if (entity->buffer <= 5) {
                entity->buffer++;
                set_frame(entity, ATTACKING_1);
            } else if (entity->buffer <= 16) {
                entity->buffer++;
                set_frame(entity, ATTACKING_2);
            } else {
                set_frame(entity, STANDING);
                entity->state = IDLE;
                entity->buffer = 0;
            }
            break;

        case WALKING:
            printf("position: (%d, %d)\n", entity->rect.x, entity->rect.y);
            if (dir & DOWN) {
                if (entity->rect.y < SCREEN_HEIGHT - entity->rect.h) {
                    entity->rect.y += entity->y_vel;
                    walk_animation(entity);
                }
            }
            if (dir & UP) {
                if (entity->rect.y > 310) {
                    entity->rect.y -= entity->y_vel;
                    walk_animation(entity);
                }
            }
            if (dir & RIGHT) {
                if (entity->rect.x < SCREEN_WIDTH - entity->rect.w) {
                    entity->rect.x += entity->x_vel;
                    entity->flip = SDL_FLIP_NONE;
                    walk_animation(entity);
                }
            }
            if (dir & LEFT) {
                if (entity->rect.x > 0) {
                    entity->rect.x -= entity->x_vel;
                    entity->flip = SDL_FLIP_HORIZONTAL;
                    walk_animation(entity);
                }
            }
            entity->state = IDLE;
            break;

        case IDLE:
            idle_animation(entity);
            break;
    }
}

char game_loop(
    struct Entity *knight,
    SDL_Joystick *joystick,
    SDL_Renderer *renderer
) {
    SDL_Texture *background_image = load_texture(
        "assets/background.png",
        renderer
    );

    unsigned int button_debounce = 150;
    unsigned int last_button_press = 0;
    while (game_running()) {
        int start_tick = SDL_GetTicks();

        /* scan buttons */
        char dir = get_direction(joystick);
        if (dir) {
            knight->state = WALKING;
        } else {
            char button = get_button(joystick);
            if ((SDL_GetTicks() - last_button_press) > button_debounce) {
                if (button) {
                    if (button & A) {
                        last_button_press = SDL_GetTicks();
                        if (knight->state != ATTACKING) {
                            knight->buffer = 0;
                            knight->state = ATTACKING;
                        }
                    }
                    if (button & B) {
                        last_button_press = SDL_GetTicks();
                    }
                    if (button & START) {
                        last_button_press = SDL_GetTicks();
                        goto cleanup;
                    }
                }
            }
        }

        player_logic(knight, dir);

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background_image, NULL, NULL);

        SDL_RenderCopyEx(
            renderer,
            knight->texture,
            &knight->texture_rect,
            &knight->rect,
            0.0, // angle
            &knight->center, // center
            knight->flip
        );

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

    struct Entity knight = create_knight(renderer);

    int status_code = game_loop(&knight, joystick, renderer);

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
