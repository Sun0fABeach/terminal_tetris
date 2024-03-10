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
  do {
    while(true) {
      const int key = read_key();
      if(key == KEY_START)
        break;
      if(key == KEY_QUIT)
        return;
    }
  } while(action_loop());
}

bool action_loop(void)
{
  while(true) {
    if(!set_new_piece())
      return false; // TODO: return true once game can loop

    bool piece_landed = false;

    do {
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
          return false;
      }

      flush_input();

    } while(!piece_landed);
  }
}

