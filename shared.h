#ifndef SHARED_H
#define SHARED_H

#include <stdint.h>

static constexpr uint8_t FIELD_HEIGHT = 18;
static constexpr uint8_t FIELD_WIDTH = 10;

static constexpr uint8_t NUM_PIECES = 7;
static constexpr uint8_t NUM_PIECE_TILES = 4;

static constexpr uint8_t MAX_REMOVABLE_LINES = 4;

typedef enum rotation {
  TOP, RIGHT, BOTTOM, LEFT
} rotation_e;

typedef enum piece_type {
  Z,
  S,
  T,
  L,
  J,
  I,
  O,
  NO_PIECE
} piece_type_e;

typedef struct coords {
  int8_t y, x; // signed to enable out of bounds checks
} coords_s;

typedef struct piece {
  piece_type_e type;
  rotation_e rotation;
  const coords_s *coords; // array size: NUM_PIECE_TILES - 1
  coords_s pos;
  bool stuck;
} piece_s;


/* helper funcs */

static inline uint8_t int_len(uint64_t val)
{
  uint8_t len = 1;
  while(val /= 10)
    len++;
  return len;
}

#endif
