#ifndef UI_H
#define UI_H

#include "shared.h"
#include <stdint.h>

bool init_ui(void);
void destroy_ui(void);
void show_start_text(void);
void draw_action(
  const piece_s piece[static 1],
  uint8_t *const lines[static FIELD_HEIGHT]
);
void draw_preview(const piece_s piece[static 1]);
void animate_line_removal(
  uint8_t *const lines[static FIELD_HEIGHT],
  const int8_t lines_completed[static MAX_REMOVABLE_LINES]
);

#endif
