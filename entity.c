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

void entity_logic(struct Entity *entity) {
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
            if (entity->dir & DOWN) {
                if (entity->rect.y < SCREEN_HEIGHT - entity->rect.h) {
                    entity->rect.y += entity->y_vel;
                    walk_animation(entity);
                }
            }
            if (entity->dir & UP) {
                if (entity->rect.y > 310) {
                    entity->rect.y -= entity->y_vel;
                    walk_animation(entity);
                }
            }
            if (entity->dir & RIGHT) {
                if (entity->rect.x < SCREEN_WIDTH - entity->rect.w) {
                    entity->rect.x += entity->x_vel;
                    entity->flip = SDL_FLIP_NONE;
                    walk_animation(entity);
                }
            }
            if (entity->dir & LEFT) {
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

void entity_render(struct Entity *entity, SDL_Renderer *renderer) {
    SDL_RenderCopyEx(
        renderer,
        entity->texture,
        &entity->texture_rect,
        &entity->rect,
        0.0, // angle
        &entity->center, // center
        entity->flip
    );
}
