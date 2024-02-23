#include <stdlib.h>
#include "ui.h"
#include "input.h"

int main(void)
{
  if(!init_ui()) {
    read_key();
    return EXIT_FAILURE;
  }

  init_input();
  read_key();
  destroy_ui();

  return EXIT_SUCCESS;
}

