#pragma once

#include <stdbool.h> // for versions previous to C23
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// 8 bytes = 64 bits, enough to represent all 64 squares of our grid
typedef uint64_t Bitboard;

// 64 bits to represent the position of each prey
// since each contains a different function,
// store them in different bitboards
extern Bitboard Prey[8];

// 64 bits to represent the position of each predator
// to prevent possible cannibalism, we store them in separate bitboards
extern Bitboard Predator[2];

const Bitboard FileABB = 0x0101010101010101ULL;
const Bitboard FileBBB = FileABB << 1;
const Bitboard FileCBB = FileABB << 2;
const Bitboard FileDBB = FileABB << 2;
const Bitboard FileEBB = FileABB << 2;
const Bitboard FileFBB = FileABB << 2;
const Bitboard FileGBB = FileABB << 2;
const Bitboard FileHBB = FileABB << 2;

const Bitboard Rank1BB = 0xFF;
const Bitboard Rank2BB = Rank1BB << (8 * 1);
const Bitboard Rank3BB = Rank1BB << (8 * 2);
const Bitboard Rank4BB = Rank1BB << (8 * 3);
const Bitboard Rank5BB = Rank1BB << (8 * 4);
const Bitboard Rank6BB = Rank1BB << (8 * 5);
const Bitboard Rank7BB = Rank1BB << (8 * 6);
const Bitboard Rank8BB = Rank1BB << (8 * 7);

typedef enum Direction : int8_t {
  NORTH = 8,
  EAST = 1,
  SOUTH = -NORTH,
  WEST = -EAST
} Direction;

typedef enum File : int8_t {
  FILE_A,
  FILE_B,
  FILE_C,
  FILE_D,
  FILE_E,
  FILE_F,
  FILE_G,
  FILE_H,
  FILE_NB
} File;

typedef enum Rank : int8_t {
  RANK_1,
  RANK_2,
  RANK_3,
  RANK_4,
  RANK_5,
  RANK_6,
  RANK_7,
  RANK_8,
  RANK_NB
} Rank;

typedef enum Piece : int8_t {
  NO_PIECE = -1,
  R_1,
  R_2,
  R_3,
  R_4,
  R_5,
  R_6,
  R_7,
  R_8,
  W_1,
  W_2
} Piece;

typedef enum Square : int8_t {
  SQ_A1,
  SQ_B1,
  SQ_C1,
  SQ_D1,
  SQ_E1,
  SQ_F1,
  SQ_G1,
  SQ_H1,
  SQ_A2,
  SQ_B2,
  SQ_C2,
  SQ_D2,
  SQ_E2,
  SQ_F2,
  SQ_G2,
  SQ_H2,
  SQ_A3,
  SQ_B3,
  SQ_C3,
  SQ_D3,
  SQ_E3,
  SQ_F3,
  SQ_G3,
  SQ_H3,
  SQ_A4,
  SQ_B4,
  SQ_C4,
  SQ_D4,
  SQ_E4,
  SQ_F4,
  SQ_G4,
  SQ_H4,
  SQ_A5,
  SQ_B5,
  SQ_C5,
  SQ_D5,
  SQ_E5,
  SQ_F5,
  SQ_G5,
  SQ_H5,
  SQ_A6,
  SQ_B6,
  SQ_C6,
  SQ_D6,
  SQ_E6,
  SQ_F6,
  SQ_G6,
  SQ_H6,
  SQ_A7,
  SQ_B7,
  SQ_C7,
  SQ_D7,
  SQ_E7,
  SQ_F7,
  SQ_G7,
  SQ_H7,
  SQ_A8,
  SQ_B8,
  SQ_C8,
  SQ_D8,
  SQ_E8,
  SQ_F8,
  SQ_G8,
  SQ_H8,
  SQ_NONE,

  SQUARE_NB = 64
} Square;

typedef struct {
  Bitboard bitboards[10]; // bitboards for all 8 prey, and one for the predators
  Bitboard occupancies[3]; // spaces occupied by pray, predators, or both
  Piece board[SQUARE_NB];
  bool preyToMove; // whose turn it is to move

} Board;
