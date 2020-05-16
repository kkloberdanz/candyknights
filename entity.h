#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>

#include "sprite.h"

enum EntityState {
    IDLE,
    WALKING,
    ATTACKING
};

struct Entity {
    SDL_Rect rect;
    SDL_Texture *texture;
    SDL_Rect texture_rect;
    int total_frames;
    int current_frame;
    SDL_RendererFlip flip;
    SDL_Point center;
    int x_vel;
    int y_vel;
    enum EntityState state;
    unsigned char buffer;
    enum IdleState idle_state;
};

void set_frame(struct Entity *entity, enum SpriteFrame frame_num);
void walk_animation(struct Entity *entity);
void idle_animation(struct Entity *entity);
struct Entity create_knight(SDL_Renderer *renderer);

#endif /* ENTITY_H */
