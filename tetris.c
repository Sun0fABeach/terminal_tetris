#include <stdlib.h>
#include <stdint.h>
#include "ui.h"
#include "input.h"
#include "shared.h"

static const coords_s rotations[NUM_PIECES][4][NUM_PIECE_TILES - 1] = {
  [S] = {
    [TOP] = { { 1, 2 }, { 2, 0 }, { 2, 1 } },
    [RIGHT] = { { 0, 0 }, { 1, 0 }, { 2, 1 } },
    [BOTTOM] = { { 1, 2 }, { 2, 0 }, { 2, 1 } },
    [LEFT] = { { 0, 0 }, { 1, 0 }, { 2, 1 } },
  }
};

int main(void)
{
  if(!init_ui()) {
    read_key();
    return EXIT_FAILURE;
  }

  init_input();

  piece_s s = {
    .type = S,
    .rotation = RIGHT,
    .pos = { 0, 3 },
    .coords = rotations[S][RIGHT]
  };
  draw_action(&s);

  int ch;
  while((ch = read_key()) != KEY_QUIT) {
    if(ch == KEY_ROTATE_LEFT) {
      switch(s.rotation) {
        case TOP: s.rotation = LEFT; break;
        case LEFT: s.rotation = BOTTOM; break;
        case BOTTOM: s.rotation = RIGHT; break;
        case RIGHT: s.rotation = TOP; break;
      }
      s.coords = rotations[S][s.rotation];
      draw_action(&s);
    } else if(ch == KEY_ROTATE_RIGHT) {
      switch(s.rotation) {
        case TOP: s.rotation = RIGHT; break;
        case RIGHT: s.rotation = BOTTOM; break;
        case BOTTOM: s.rotation = LEFT; break;
        case LEFT: s.rotation = TOP; break;
      }
      s.coords = rotations[S][s.rotation];
      draw_action(&s);
    }
  }

  destroy_ui();

  return EXIT_SUCCESS;
}

