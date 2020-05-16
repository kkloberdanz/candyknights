#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <SDL2/SDL.h>

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

char *get_button_str(enum Button button);
int translate_controller_button(int button_num);
char get_button(SDL_Joystick *joystick);
char get_direction(SDL_Joystick *joystick);

#endif /* CONTROLLER_H */
