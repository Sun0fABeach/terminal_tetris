#include "input.h"
#include <ncurses.h>

void init_input(void)
{
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
}

int read_key(void)
{
  return getch();
}

