#include "game.h"
#include "ui.h"
#include "shared.h"

static const coords_s rotations[NUM_PIECES][4][NUM_PIECE_TILES - 1] = {
  [S] = {
    [TOP] = { { 1, 2 }, { 2, 0 }, { 2, 1 } },
    [RIGHT] = { { 0, 0 }, { 1, 0 }, { 2, 1 } },
    [BOTTOM] = { { 1, 2 }, { 2, 0 }, { 2, 1 } },
    [LEFT] = { { 0, 0 }, { 1, 0 }, { 2, 1 } },
  }
};

static piece_s current_piece;

bool init_game(void)
{
  return init_ui();
}

void tear_down_game(void)
{
  destroy_ui();
}

void set_new_piece(void)
{
  current_piece = (piece_s) {
    .type = S,
    .rotation = RIGHT,
    .pos = { 0, 3 },
    .coords = rotations[S][RIGHT]
  };
  draw_action(&current_piece);
}

void rotate_piece_left(void)
{
  switch(current_piece.rotation) {
    case TOP: current_piece.rotation = LEFT; break;
    case LEFT: current_piece.rotation = BOTTOM; break;
    case BOTTOM: current_piece.rotation = RIGHT; break;
    case RIGHT: current_piece.rotation = TOP; break;
  }
  current_piece.coords = rotations[S][current_piece.rotation];
  draw_action(&current_piece);
}

void rotate_piece_right(void)
{
  switch(current_piece.rotation) {
    case TOP: current_piece.rotation = RIGHT; break;
    case RIGHT: current_piece.rotation = BOTTOM; break;
    case BOTTOM: current_piece.rotation = LEFT; break;
    case LEFT: current_piece.rotation = TOP; break;
  }
  current_piece.coords = rotations[S][current_piece.rotation];
  draw_action(&current_piece);
}
