#include <SDL2/SDL.h>

#include "controller.h"
#include "constants.h"

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
