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

#include <SDL2/SDL.h>
#include <stdbool.h>

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

char *direction_str[] = {
    "UP",
    "DOWN",
    "LEFT",
    "RIGHT"
};

enum Button {
    B = 0,
    A = 1,
    START = 9,
    SELECT = 8
};

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

int get_button(SDL_Joystick *joystick) {
    int num_buttons = SDL_JoystickNumButtons(joystick);
    for (int i = 0; i < num_buttons; i++) {
        if (SDL_JoystickGetButton(joystick, i)) {
            return i;
        }
    }
    return -1;
}

int get_direction(SDL_Joystick *joystick) {
    int num_axes = SDL_JoystickNumAxes(joystick);
    for (int i = 0; i < num_axes; i++) {
        int axis = SDL_JoystickGetAxis(joystick, i);
        if (axis) {
            if (i == 1) {
                if (axis < 0) {
                    return UP;
                } else {
                    return DOWN;
                }
            } else if (i == 0) {
                if (axis < 0) {
                    return LEFT;
                } else {
                    return RIGHT;
                }
            }
        }
    }
    return -1;
}

SDL_Surface *load_image(char *filename) {
    SDL_Surface *image_loaded = NULL;
    SDL_Surface *processed_image = NULL;

    image_loaded = SDL_LoadBMP(filename);

    if (image_loaded != NULL) {
        processed_image = SDL_DisplayFormat(image_loaded);
        SDL_FreeSurface(image_loaded);

        if (processed_image != NULL) {
            int color_key = SDL_MapRGB(processed_image->format, 0xFF, 0, 0xFF);
            SDL_SetColorKey(processed_image, SDL_SRCCOLORKEY, color_key);
        }
    }
    return processed_image;
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

int main(void) {
    /* initialize */
    SDL_Init(SDL_INIT_EVERYTHING);
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
        goto quit;
    }

    int num_buttons = SDL_JoystickNumButtons(joystick);
    int num_hats = SDL_JoystickNumHats(joystick);
    printf("num buttons: %d\n", num_buttons);
    printf("num hats: %d\n", num_hats);
    printf("num axis: %d\n", SDL_JoystickNumAxes(joystick));
    printf("num balls: %d\n", SDL_JoystickNumBalls(joystick));

    while (game_running()) {
        int button = get_button(joystick);
        if (button >= 0) {
            printf("button: %s\n", get_button_str(button));
        }

        int dir = get_direction(joystick);
        if (dir >= 0) {
            printf("direction: %s\n", direction_str[dir]);
        }
        SDL_Delay(20);
    }

close_joystick:
    SDL_JoystickClose(joystick);
quit:
    SDL_Quit();
}
