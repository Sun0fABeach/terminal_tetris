#ifndef INPUT_H
#define INPUT_H

#include <ncurses.h>

constexpr int KEY_QUIT = 'q';
constexpr int KEY_START = 's';
constexpr int KEY_ROTATE_LEFT = 'a';
constexpr int KEY_ROTATE_RIGHT = 'd';
constexpr int KEY_MOVE_LEFT = KEY_LEFT;
constexpr int KEY_MOVE_RIGHT = KEY_RIGHT;
constexpr int KEY_PUSH_DOWN = KEY_DOWN;

void init_input(void);
int read_key(void);
void flush_input(void);

#endif

