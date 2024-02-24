#ifndef SHARED_H
#define SHARED_H

constexpr uint8_t NUM_PIECES = 7;
constexpr uint8_t NUM_PIECE_TILES = 4;

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
  O
} piece_type_e;

typedef struct coords {
  uint8_t y, x;
} coords_s;

typedef struct piece {
  const piece_type_e type;
  rotation_e rotation;
  coords_s pos;
  const coords_s *coords; // array size: NUM_PIECE_TILES - 1
} piece_s;

#endif
