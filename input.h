#ifndef INPUT_H
#define INPUT_H

#include <ncurses.h>

static constexpr int KEY_QUIT = 'q';
static constexpr int KEY_START = 's';
static constexpr int KEY_ROTATE_LEFT = 'a';
static constexpr int KEY_ROTATE_RIGHT = 'd';
static constexpr int KEY_MOVE_LEFT = KEY_LEFT;
static constexpr int KEY_MOVE_RIGHT = KEY_RIGHT;
static constexpr int KEY_PUSH_DOWN = KEY_DOWN;

void init_input(void);
int read_key(void);
void flush_input(void);

#endif

