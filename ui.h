#ifndef UI_H
#define UI_H

#include "shared.h"
#include <stdint.h>

bool init_ui(void);
void destroy_ui(void);
void draw_action(
  const piece_s piece[static 1],
  const uint8_t field[static FIELD_HEIGHT][FIELD_WIDTH]
);
void draw_preview(const piece_s piece[static 1]);
void animate_line_removal(
  const int8_t lines_completed[static MAX_REMOVABLE_LINES]
);

#endif
