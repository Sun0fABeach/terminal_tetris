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
    if(ch == KEY_ROTATE_LEFT) {
      rotate_piece_left();
    } else if(ch == KEY_ROTATE_RIGHT) {
      rotate_piece_right();
    }
  }

  tear_down_game();

  return EXIT_SUCCESS;
}

