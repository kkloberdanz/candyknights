#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX(A, B) (((A) > (B)) ? (A) : (B))

enum Constants {
    SCREEN_WIDTH = 800,
    SCREEN_HEIGHT = 600,
    MAX_VELOCITY = 20,
    MIN_VELOCITY = 10,
    WINNING_SCORE = 10,
};

enum GameState {
    END_GAME,
    PLAY_GAME
};

enum Direction {
    UP = 1,
    DOWN = 2,
    LEFT = 4,
    RIGHT = 8
};

#endif /* CONSTANTS_H */
