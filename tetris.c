#include <stdlib.h>
#include "input.h"
#include "game.h"

void game_loop(void);
bool action_loop(void);

int main(void)
{
  if(!init_game()) {
    getch();
    return EXIT_FAILURE;
  }

  init_input();
  greet_player();
  game_loop();
  tear_down_game();

  return EXIT_SUCCESS;
}

void game_loop(void)
{
  while(true) {
    while(true) {
      const int key = read_key();
      if(key == KEY_START)
        break;
      if(key == KEY_QUIT)
        return;
    }

    setup_action();

    if(!action_loop())
      return;
  }
}

bool action_loop(void)
{
  set_new_piece();

  while(true) {
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
        if(!push_piece_down()) {
          if(game_is_lost()) {
            game_over();
            return true;
          }
          set_new_piece();
        }
        break;
      case KEY_QUIT:
        return false;
    }

    flush_input();
  }
}

