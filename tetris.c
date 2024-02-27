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

  bool quit = false;

  while(!quit) {
    set_new_piece();

    bool piece_landed = false;

    while(!piece_landed && !quit) {
      switch(getch()) {
        case KEY_ROTATE_LEFT:
          rotate_piece_left(); break;
        case KEY_ROTATE_RIGHT:
          rotate_piece_right(); break;
        case KEY_MOVE_LEFT:
          move_piece_left(); break;
        case KEY_MOVE_RIGHT:
          move_piece_right(); break;
        case KEY_PUSH_DOWN:
          if(!push_piece_down())
            piece_landed = true;
          break;
        case KEY_QUIT:
          quit = true;
          break;
      }
    }
  }

  tear_down_game();

  return EXIT_SUCCESS;
}

