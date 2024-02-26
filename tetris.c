#include <stdlib.h>
#include "input.h"
#include "game.h"

int main(void)
{
  if(!init_game()) {
    read_key();
    return EXIT_FAILURE;
  }

  init_input();
  set_new_piece();

  int ch;
  while((ch = read_key()) != KEY_QUIT) {
    switch(ch) {
      case KEY_ROTATE_LEFT:
        rotate_piece_left(); break;
      case KEY_ROTATE_RIGHT:
        rotate_piece_right(); break;
      case KEY_MOVE_LEFT:
        move_piece_left(); break;
      case KEY_MOVE_RIGHT:
        move_piece_right(); break;
      case KEY_PUSH_DOWN:
        push_piece_down(); break;
    }
  }

  tear_down_game();

  return EXIT_SUCCESS;
}

