#ifndef GAME_H
#define GAME_H

typedef enum move_result {
  MOVE_BLOCKED,
  MOVE_DONE,
  PIECE_PLACED,
  PIECE_STUCK,
} move_result_e;

bool init_game(void);
void tear_down_game(void);
void greet_player(void);
void setup_action(void);
bool game_is_lost(void);
void game_over(void);
void set_new_piece(void);
move_result_e rotate_piece_left(void);
move_result_e rotate_piece_right(void);
move_result_e move_piece(int8_t y, int8_t x);

static inline move_result_e move_piece_left(void)
{
  return move_piece(0, -1);
}

static inline move_result_e move_piece_right(void)
{
  return move_piece(0, 1);
}

static inline move_result_e push_piece_down(void)
{
  return move_piece(1, 0);
}

#endif
