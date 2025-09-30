#include "bitboard.h"
#include "pgm_io.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const int renji_params[8] = {4, 3, 3, 2, 3, 4, 5, 8};
const int renji_B[8] = {3, 4, 1, 2, 5, 8, 12, 9};

uint64_t pack_bytes_to_u64(char bytes[8]) {
  return ((uint64_t)(bytes[0]) << 56) | ((uint64_t)(bytes[1]) << 48) |
         ((uint64_t)(bytes[2]) << 40) | ((uint64_t)(bytes[3]) << 32) |
         ((uint64_t)(bytes[4]) << 24) | ((uint64_t)(bytes[5]) << 16) |
         ((uint64_t)(bytes[6]) << 8) | ((uint64_t)(bytes[7]));
}

char *unpack_u64_to_bytes(uint64_t u64) {
  char *data = malloc(8 * sizeof(char));
  if (!data) {
    perror("Could not assign memory.\n");
    return NULL;
  }
  for (int i = 0; i < 8; i++) {
    // 0xF = 1111, so we shift this value by 4 each iteration
    // and use the & operator to select only those specific bytes from
    // u64
    data[7 - i] = u64 & (0xF << 4 * i);
  }
  return data;
}

Piece piece_on(Board *board, Square sq) { return board->board[sq]; }

void clearBit(Bitboard *bb, Square square) { *bb &= ~(1ULL << square); }

void setBit(Bitboard *bb, Square square) { *bb |= (1ULL << square); }

static inline int lsb(Bitboard bb) { return __builtin_ctzll(bb); }

static inline int popCount(Bitboard bb) { return __builtin_popcountll(bb); }

static inline Square nth_bit(Bitboard bb, int n) {
  while (n--) {
    bb &= bb - 1; // remove lsb until nth one
  }
  return lsb(bb); // return new lsb (nth one)
}

static inline bool valid_sq(Square square) {
  if (square < 0 || square > 63) {
    return false;
  }
  return true;
}

uint64_t renyi_map(uint64_t xj, unsigned int B, unsigned int k) {
  return B * xj + (xj >> k);
}

void respawn_prey(Board *board, Piece p) {
  if (p < W_1) {
    Square sq = 0;
    do {
      sq = rand() % SQUARE_NB;
    } while (board->occupancies[2] & (1ULL << sq));
    setBit(&board->bitboards[p], sq);
    setBit(&board->occupancies[0], sq);
    setBit(&board->occupancies[2], sq);
    board->board[sq] = p;
  }
}

bool can_move(Board *bb, Piece p, Square from, Square to) {
  if (((1ULL << from) & FileHBB) && ((1ULL << to) & FileABB)) {
    return false;
  }
  if (((1ULL << from) & FileABB) && ((1ULL << to) & FileHBB)) {
    return false;
  }
  // if it is a rabbit
  if (p < W_1) {
    if (bb->occupancies[2] & (1ULL << to)) {
      return false;
    }
    return true;
  }
  // if it is a wolf

  if (bb->occupancies[1] & (1ULL << to)) {
    return false;
  }
  return true;
}

/*

randomizes the initial position of a bitboard

 */
void initialize_board(Board *board) {

  for (Piece p = R_1; p <= W_2; ++p) {
    board->bitboards[p] = 0ULL;
  }
  board->occupancies[0] = 0ULL;
  board->occupancies[1] = 0ULL;
  board->occupancies[2] = 0ULL;

  board->preyToMove = true;
  for (Square sq = SQ_A1; sq < SQUARE_NB; ++sq) {
    board->board[sq] = NO_PIECE;
  }

  for (Piece p = R_1; p <= W_2; ++p) {
    Square sq = 0;
    do {
      sq = rand() % SQUARE_NB;
    } while (board->occupancies[2] & (1ULL << sq));
    setBit(&board->bitboards[p], sq);
    if (p < W_1) {
      setBit(&board->occupancies[0], sq);
    } else {
      setBit(&board->occupancies[1], sq);
    }
    setBit(&board->occupancies[2], sq);
    board->board[sq] = p;
  }
}

/*

prints the board struct passed

** stockfish design insipiration **

 */
void print_board(Board *board) {
  printf("\033[H");
  char pieceSymbols[10] = {'s', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'L', 'K'};
  int targetStrLen = 10;
  const char *padding = "00000000000000000000000000000000";
  int padLen = targetStrLen - strlen("12345");

  printf("+---+---+---+---+---+---+---+---+\n");
  for (Rank r = RANK_8; r >= RANK_1; --r) {
    for (File f = FILE_A; f <= FILE_H; ++f) {
      int square = r * 8 + f;
      char symbol = ' ';
      for (Piece p = R_1; p <= W_2; ++p) {
        if (board->bitboards[p] & (1ULL << square)) {
          symbol = pieceSymbols[p];
          break;
        }
      }
      printf("| %c ", symbol);
    }
    printf("| %d", r + 1);
    printf("\n+---+---+---+---+---+---+---+---+\n");
  }
  printf("  a   b   c   d   e   f   g   h  \n");
  for (int i = 0; i < 8; i++) {
    printf("prey %i = %lu%*.*s\n", i, board->bitboards[i], padLen, padLen,
           padding);
  }
  printf("occ 0 = %lu\n", board->occupancies[0]);
  fflush(stdout);
}

uint64_t make_move(Board *board, Square from, Square to) {
  // retrieve piece to be moved
  Piece p_from = piece_on(board, from);
  Piece p_to = piece_on(board, to);

  // if moving to a predator, warn caller
  if (board->occupancies[1] & (1ULL << to)) {
    return -2;
  }

  // if prey is the one moving
  if (p_from < W_1) {
    // prevent prey from moving to occupied prey squares
    if (board->occupancies[0] & (1ULL << to)) {
      return -2;
    }
    clearBit(&board->occupancies[0], from);
    setBit(&board->occupancies[0], to);
  } else { // if predator is moving
    clearBit(&board->occupancies[1], from);
    setBit(&board->occupancies[1], to);
  }
  // general occupancy update
  clearBit(&board->occupancies[2], from);
  setBit(&board->occupancies[2], to);

  // piecewise bitboard update
  clearBit(&board->bitboards[p_from], from);
  setBit(&board->bitboards[p_from], to);

  // board with piece value update
  board->board[from] = NO_PIECE;
  board->board[to] = p_from;

  // check if there was a captures (must be a prey after previous inspections)
  if (p_to != NO_PIECE) {
    board->bitboards[p_to] = 0ULL;
    clearBit(&board->occupancies[0], to);
    // respawn captures prey
    respawn_prey(board, p_to);

    uint64_t x = renyi_map(to, renji_B[p_to], renji_params[p_to]);

    return x;
  }
  return NO_PIECE;
}

uint64_t move_piece(Board *board, Piece p) {
  if (!board->bitboards[p]) {
    return 0;
  }
  Square from = lsb(board->bitboards[p]);
  Bitboard moves = 0ULL;
  if (valid_sq(from + NORTH))
    moves |= 1ULL << (from + NORTH);
  if (valid_sq(from + SOUTH))
    moves |= 1ULL << (from + SOUTH);
  if (valid_sq(from + EAST) && !((1ULL << from) & FileHBB))
    moves |= 1ULL << (from + EAST);
  if (valid_sq(from + WEST) && !((1ULL << from) & FileABB))
    moves |= 1ULL << (from + WEST);

  if (p < W_1) {
    // prey cannot move to where a predator is, nor another prey's sq
    moves &= ~board->occupancies[2];
    // if prey cannot move, do nothing
    if (!moves) {
      return 0;
    }
  } else {
    // predator cannot move to another predators square
    moves &= ~board->occupancies[1];

    // this should never be the case if there are only 2 predators
    if (!moves) {
      return 0;
    }
    // check for adjacent prey
    if (moves & board->occupancies[0]) {
      moves &= board->occupancies[0];

      // go to smallest prey (in bits)
      Square to = lsb(moves);
      return make_move(board, from, to);
    }
    // if no adjacent prey are found, move randomly
  }
  int move_count = popCount(moves);
  int choice = rand() % move_count;   // only choose from available directions
  Square to = nth_bit(moves, choice); // get choice'th lsb

  return make_move(board, from, to);
}

void move_prey(Board *board) {
  for (Piece p = R_1; p < W_1; ++p) {
    move_piece(board, p);
  }
}

void cypher(char *file_in, char *file_out, Board *board) {
  int rows, cols;
  unsigned char **image;
  image = pgmRead(file_in, &rows, &cols);
  if (image == NULL) {
    printf("Error pgmRead\n");
    exit(1);
  }
  unsigned char **image_out;

  uint64_t renji;
  char im_data[8];
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols - 8; j += 8) {
      for (Piece p = W_1; p <= W_2; ++p) {
        renji = move_piece(board, p);
        if (renji > 0) {
          for (int k = 0; k < 8; k++) {
            im_data[k] = image[i][j + k];
          }
          uint64_t packed_bytes = pack_bytes_to_u64(im_data);
          uint64_t new_data = packed_bytes ^ renji;
          char *new_bytes = unpack_u64_to_bytes(new_data);
          for (int k = 0; k < 8; k++) {
            image_out[i][j + k] = new_bytes[k];
          }
          free(new_bytes);
        }
      }
    }
  }
}

int main() {
  srand(time(NULL));
  Board *board = malloc(sizeof(Board));
  initialize_board(board);
  print_board(board);
  while (true) {
    move_prey(board);
    print_board(board);
  }
  free(board);
  return 0;
}
