#ifndef SPRITE_H
#define SPRITE_H

enum SpriteFrame {
    STANDING,
    WALKING_1,
    WALKING_2,
    BLINK_1,
    BLINK_2,
    BLINK_3,
    LOOK_DOWN,
    LOOK_UP,
    ATTACKING_1,
    ATTACKING_2
};

enum IdleState {
    BLINKING,
    LOOK_AROUND,
    STILL
};

#endif /* SPRITE_H */
