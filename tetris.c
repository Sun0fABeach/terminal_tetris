#include <stdlib.h>
#include <stdint.h>
#include "ui.h"
#include "input.h"
#include "shared.h"

int main(void)
{
  if(!init_ui()) {
    read_key();
    return EXIT_FAILURE;
  }

  init_input();

  piece_s s = {
    .rotation = RIGHT,
    .pos = { 0, 3 },
    .coords = { { 1, 2 }, { 2, 0 }, { 2, 1 } }
  };
  draw_action(&s);

  read_key();

  s = (piece_s) {
   .rotation = TOP,
   .pos = { 0, 3 },
   .coords = { { 0, 0 }, { 1, 0 }, { 2, 1 } }
  };
  draw_action(&s);

  read_key();

  destroy_ui();

  return EXIT_SUCCESS;
}

