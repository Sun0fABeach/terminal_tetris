#ifndef UI_H
#define UI_H

#include "shared.h"

bool init_ui(void);
void destroy_ui(void);
void draw_action(
  const piece_s piece[const static 1],
  const uint8_t field[static FIELD_HEIGHT][FIELD_WIDTH]
);

#endif
