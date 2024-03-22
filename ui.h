#ifndef UI_H
#define UI_H

#include "shared.h"
#include <stdint.h>

bool init_ui(void);
void destroy_ui(void);
void show_start_text(void);
void show_game_over_text(uint32_t score);
void set_score_text(
  uint32_t score, uint8_t level, uint16_t lines, uint16_t num_tetris
);
void draw_action(
  const piece_s piece[static 1],
  uint8_t *const lines[static FIELD_HEIGHT]
);
void draw_preview(const piece_s piece[static 1]);
void animate_line_removal(
  uint8_t *const lines[static FIELD_HEIGHT],
  const int8_t lines_completed[static MAX_REMOVABLE_LINES]
);
void animate_game_over(void);

#endif
