#include <stdlib.h>
#include "input.h"
#include "game.h"

int main(void)
{
  if(!init_game()) {
    getch();
    return EXIT_FAILURE;
  }

  init_input();
  greet_player();

  while(true) {
    const int key = read_key();
    if(key == KEY_START)
      break;
    if(key == KEY_QUIT)
      goto quit;
  }

  while(true) {
    set_new_piece();

    bool piece_landed = false;

    while(!piece_landed) {
      const int key = read_key();

      switch(key) {
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
          goto quit;
      }

      flush_input();
    }
  }

quit:
  tear_down_game();
  return EXIT_SUCCESS;
}

