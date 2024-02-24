#ifndef INPUT_H
#define INPUT_H

#include <ncurses.h>

constexpr int KEY_QUIT = 'q';
constexpr int KEY_ROTATE_LEFT = KEY_LEFT;
constexpr int KEY_ROTATE_RIGHT = KEY_RIGHT;

void init_input(void);
int read_key(void);

#endif

