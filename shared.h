#ifndef SHARED_H
#define SHARED_H

constexpr uint8_t NUM_PIECE_TILES = 4;

typedef enum rotation {
  TOP, RIGHT, BOTTOM, LEFT
} rotation_e;

typedef struct coords {
  uint8_t y, x;
} coords_s;

typedef struct piece {
  rotation_e rotation;
  coords_s pos;
  coords_s coords[NUM_PIECE_TILES - 1]; // center tile always has same coords (1, 1)
} piece_s;

#endif
