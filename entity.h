#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>

#include "sprite.h"
#include "constants.h"

enum EntityState {
    IDLE,
    WALKING,
    ATTACKING
};

struct Entity {
    SDL_Rect rect;
    SDL_Rect hitbox;
    SDL_Rect attack_box;
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
    unsigned char health;
    unsigned char handicap;
    unsigned char max_handicap;
    enum IdleState idle_state;
    enum Direction dir;
    int actions;
    int sleep_actions;
    int max_actions;
    int team;
};

void set_frame(struct Entity *entity, enum SpriteFrame frame_num);

void walk_animation(struct Entity *entity);

void idle_animation(struct Entity *entity);

struct Entity *create_knight(SDL_Renderer *renderer, struct Entity *knight);

enum GameState handle_player_input(
    struct Entity *player,
    SDL_Joystick *joystick
);

void entity_logic(struct Entity *entity, struct Entity *entities[], size_t nun_entities);

void entity_render(struct Entity *entity, SDL_Renderer *renderer);

void enemy_ai_logic(struct Entity *player, struct Entity *enemy);

void entity_set_pos(struct Entity *entity, int x, int y);

#endif /* ENTITY_H */
