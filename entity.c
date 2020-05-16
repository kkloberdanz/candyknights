#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "entity.h"
#include "sprite.h"
#include "textures.h"
#include "controller.h"
#include "constants.h"

void set_frame(struct Entity *entity, enum SpriteFrame frame_num) {
    entity->current_frame = frame_num;
    entity->texture_rect.x = entity->current_frame * entity->texture_rect.w;
}

void walk_animation(struct Entity *entity) {
    if (entity->buffer <= 7) {
        entity->buffer++;
        set_frame(entity, WALKING_1);
    } else if (entity->buffer <= 14) {
        entity->buffer++;
        set_frame(entity, WALKING_2);
    } else {
        set_frame(entity, STANDING);
        entity->state = IDLE;
        entity->buffer = 0;
    }
}

enum IdleState get_rand_idle_state() {
    return rand() % STILL;
}

void idle_animation(struct Entity *entity) {
    switch (entity->idle_state) {
        case BLINKING:
            if (entity->buffer <= 200) {
                entity->buffer++;
                set_frame(entity, STANDING);
            } else if (entity->buffer <= 205) {
                entity->buffer++;
                set_frame(entity, BLINK_1);
            } else if (entity->buffer <= 210) {
                entity->buffer++;
                set_frame(entity, BLINK_2);
            } else if (entity->buffer <= 215) {
                entity->buffer++;
                set_frame(entity, BLINK_3);
            } else if (entity->buffer <= 220) {
                entity->buffer = 0;
                set_frame(entity, STANDING);
                entity->idle_state = get_rand_idle_state();
            }
            break;

        case LOOK_AROUND:
            if (entity->buffer < 50) {
                entity->buffer++;
                set_frame(entity, STANDING);
            } else if (entity->buffer <= 100) {
                entity->buffer++;
                set_frame(entity, LOOK_UP);
            } else if (entity->buffer <= 150) {
                entity->buffer++;
                set_frame(entity, LOOK_DOWN);
            } else if (entity->buffer <= 200) {
                entity->buffer = 0;
                set_frame(entity, STANDING);
                entity->idle_state = get_rand_idle_state();
            }
            break;

        case STILL:
            if (entity->buffer < 200) {
                entity->buffer++;
            } else {
                entity->buffer = 0;
                entity->idle_state = get_rand_idle_state();
            }
            set_frame(entity, STANDING);
            break;
    }
}

struct Entity create_knight(SDL_Renderer *renderer) {
    struct Entity knight = {
        .rect = {
            .x = 300,
            .y = 400,
            .w = 128,
            .h = 128
        },
        .texture = load_texture("assets/knight.png", renderer),
        .texture_rect = {
            .x = 0,
            .y = 0,
            .w = 64,
            .h = 64
        },
        .total_frames = 10,
        .current_frame = 0,
        .flip = SDL_FLIP_NONE,
        .center = {
            .x = 64,
            .y = 64
        },
        .x_vel = 10,
        .y_vel = 10,
        .state = IDLE,
        .buffer = 0,
        .idle_state = STILL
    };
    return knight;
}

enum GameState handle_player_input(
    struct Entity *player,
    SDL_Joystick *joystick
) {
    unsigned int last_button_press = 0;
    const unsigned int button_debounce = 150;

    /* scan buttons */
    player->dir = get_direction(joystick);
    if (player->dir) {
        player->state = WALKING;
    } else {
        char button = get_button(joystick);
        if ((SDL_GetTicks() - last_button_press) > button_debounce) {
            if (button) {
                if (button & A) {
                    last_button_press = SDL_GetTicks();
                    if (player->state != ATTACKING) {
                        player->buffer = 0;
                        player->state = ATTACKING;
                    }
                }
                if (button & B) {
                    last_button_press = SDL_GetTicks();
                }
                if (button & START) {
                    last_button_press = SDL_GetTicks();
                    return END_GAME;
                }
            }
        }
    }
    return PLAY_GAME;
}
