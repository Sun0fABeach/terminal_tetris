#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "game.h"
#include "ui.h"
#include "shared.h"

static piece_s create_piece(void);
static bool try_rotation(rotation_e new_rotation);
static inline coords_s change_pos(coords_s pos, int8_t y, int8_t x);
static bool causes_collision(
  coords_s piece_pos,
  const coords_s piece_coords[static NUM_PIECE_TILES - 1]
);
static inline bool check_collision(coords_s coord);
static void incorporate_piece(void);

static const coords_s rotations[NUM_PIECES][4][NUM_PIECE_TILES - 1] = {
  [Z] = {
    [TOP] = { { 0, 1 }, { 1, 0 }, { 2, 0 } },
    [RIGHT] = { { 1, 0 }, { 2, 1 }, { 2, 2 } },
    [BOTTOM] = { { 0, 1 }, { 1, 0 }, { 2, 0 } },
    [LEFT] = { { 1, 0 }, { 2, 1 }, { 2, 2 } },
  },
  [S] = {
    [TOP] = { { 0, 0 }, { 1, 0 }, { 2, 1 } },
    [RIGHT] = { { 1, 2 }, { 2, 0 }, { 2, 1 } },
    [BOTTOM] = { { 0, 0 }, { 1, 0 }, { 2, 1 } },
    [LEFT] = { { 1, 2 }, { 2, 0 }, { 2, 1 } },
  },
  [T] = {
    [TOP] = { { 0, 1 }, { 1, 0 }, { 1, 2 } },
    [RIGHT] = { { 0, 1 }, { 1, 2 }, { 2, 1 } },
    [BOTTOM] = { { 1, 0 }, { 1, 2 }, { 2, 1 } },
    [LEFT] = { { 0, 1 }, { 1, 0 }, { 2, 1 } },
  },
  [L] = {
    [TOP] = { { 0, 1 }, { 2, 1 }, { 2, 2 } },
    [RIGHT] = { { 1, 0 }, { 1, 2 }, { 2, 0 } },
    [BOTTOM] = { { 0, 0 }, { 0, 1 }, { 2, 1 } },
    [LEFT] = { { 0, 2 }, { 1, 0 }, { 1, 2 } },
  },
  [J] = {
    [TOP] = { { 0, 1 }, { 2, 0 }, { 2, 1 } },
    [RIGHT] = { { 0, 0 }, { 1, 0 }, { 1, 2 } },
    [BOTTOM] = { { 0, 1 }, { 0, 2 }, { 2, 1 } },
    [LEFT] = { { 1, 0 }, { 1, 2 }, { 2, 2 } },
  },
  [I] = {
    [TOP] = { { -1, 1 }, { 0, 1 }, { 2, 1 } },
    [RIGHT] = { { 1, 0 }, { 1, 2 }, { 1, 3 } },
    [BOTTOM] = { { -1, 1 }, { 0, 1 }, { 2, 1 } },
    [LEFT] = { { 1, 0 }, { 1, 2 }, { 1, 3 } },
  },
  [O] = {
    [TOP] = { { 1, 2 }, { 2, 1 }, { 2, 2 } },
    [RIGHT] = { { 1, 2 }, { 2, 1 }, { 2, 2 } },
    [BOTTOM] = { { 1, 2 }, { 2, 1 }, { 2, 2 } },
    [LEFT] = { { 1, 2 }, { 2, 1 }, { 2, 2 } },
  }
};

static uint8_t field[FIELD_HEIGHT][FIELD_WIDTH];

static piece_s current_piece, next_piece;


bool init_game(void)
{
  srand(time(NULL));
  memset(field, NO_PIECE, FIELD_HEIGHT * FIELD_WIDTH);
  next_piece.type = NO_PIECE;
  return init_ui();
}

void tear_down_game(void)
{
  destroy_ui();
}

void set_new_piece(void)
{
  if(next_piece.type == NO_PIECE) {
    current_piece = create_piece();
  } else {
    current_piece = next_piece;
  }
  next_piece = create_piece();

  draw_preview(&next_piece);
  draw_action(&current_piece, field);
}

static piece_s create_piece(void)
{
  constexpr int8_t start_y = 0;
  constexpr int8_t start_x = 3;
  const piece_type_e piece_type = rand() % NUM_PIECES;
  rotation_e rotation;

  switch(piece_type) {
    case S: case L: case I:
      rotation = RIGHT;
      break;
    case Z: case J:
      rotation = LEFT;
      break;
    case T: case O:
      rotation = BOTTOM;
      break;
    default:
      break;
  }

  return (piece_s) {
    .type = piece_type,
    .rotation = rotation,
    .pos = { start_y, start_x },
    .coords = rotations[piece_type][rotation]
  };
}

bool rotate_piece_left(void)
{
  rotation_e new_rotation;

  switch(current_piece.rotation) {
    case TOP: new_rotation = LEFT; break;
    case LEFT: new_rotation = BOTTOM; break;
    case BOTTOM: new_rotation = RIGHT; break;
    case RIGHT: new_rotation = TOP; break;
  }

  return try_rotation(new_rotation);
}

bool rotate_piece_right(void)
{
  rotation_e new_rotation;

  switch(current_piece.rotation) {
    case TOP: new_rotation  = RIGHT; break;
    case RIGHT: new_rotation = BOTTOM; break;
    case BOTTOM: new_rotation = LEFT; break;
    case LEFT: new_rotation = TOP; break;
  }

  return try_rotation(new_rotation);
}

static bool try_rotation(const rotation_e new_rotation)
{
  const coords_s *const new_rot_coords =
    rotations[current_piece.type][new_rotation];

  if(causes_collision(current_piece.pos, new_rot_coords))
    return false;

  current_piece.rotation = new_rotation;
  current_piece.coords = new_rot_coords;
  draw_action(&current_piece, field);

  return true;
}

bool move_piece(const int8_t y, const int8_t x)
{
  const coords_s new_pos = change_pos(current_piece.pos, y, x);
  if(causes_collision(new_pos, current_piece.coords)) {
    if(y == 1) // downwards move
      incorporate_piece();
    return false;
  }

  current_piece.pos = new_pos;
  draw_action(&current_piece, field);
  return true;
}

static inline coords_s change_pos(
  const coords_s pos, const int8_t y, const int8_t x
)
{
  return (coords_s) { pos.y + y, pos.x + x };
}

static bool causes_collision(
  const coords_s piece_pos,
  const coords_s piece_coords[static NUM_PIECE_TILES - 1]
)
{
  if(check_collision((coords_s) { piece_pos.y + 1, piece_pos.x + 1 }))
    return true;

  for(uint8_t i = 0; i < NUM_PIECE_TILES - 1; i++) {
    if(check_collision((coords_s) {
      piece_pos.y + piece_coords[i].y,
      piece_pos.x + piece_coords[i].x
    })) {
      return true;
    }
  }

  return false;
}

static inline bool check_collision(const coords_s coord)
{
  if(coord.y < 0)
    return false;
  if(coord.y >= FIELD_HEIGHT)
    return true;
  if(coord.x < 0 || coord.x >= FIELD_WIDTH)
    return true;
  if(field[coord.y][coord.x] != NO_PIECE)
    return true;

  return false;
}

static void incorporate_piece(void)
{
  const int8_t y = current_piece.pos.y;
  const int8_t x = current_piece.pos.x;
  field[y + 1][x + 1] = current_piece.type;

  for(uint8_t i = 0; i < NUM_PIECE_TILES - 1; i++) {
    const int8_t tile_y = y + current_piece.coords[i].y;
    const int8_t tile_x = x + current_piece.coords[i].x;
    // I piece can go out of bounds if rotated upright at starting pos
    if(tile_y >= 0)
      field[tile_y][tile_x] = current_piece.type;
  }
}

