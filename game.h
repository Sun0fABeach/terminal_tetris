#ifndef GAME_H
#define GAME_H

bool init_game(void);
void tear_down_game(void);
void set_new_piece(void);
bool rotate_piece_left(void);
bool rotate_piece_right(void);
bool move_piece(int8_t y, int8_t x);

static inline bool move_piece_left(void)
{
  return move_piece(0, -1);
}

static inline bool move_piece_right(void)
{
  return move_piece(0, 1);
}

static inline bool push_piece_down(void)
{
  return move_piece(1, 0);
}

#endif
