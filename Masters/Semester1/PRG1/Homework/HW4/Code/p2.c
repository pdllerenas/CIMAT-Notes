#include "bitboard.h"
#include "pgm_io.h"
#include "wav_io.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const int renji_params[8] = {4, 3, 3, 2, 3, 4, 5, 8};
const uint64_t renji_B[8] = {3424232302134182413, 2123232302134188241,
                             1234222234124564743, 2422312345234582412,
                             3234672342432428315, 5578223424234234818,
                             4234244187724234414, 2423424423422124319};
uint64_t pack_bytes_to_u64(char bytes[8]) {
  return ((uint64_t)((uint8_t)bytes[0]) << 56) |
         ((uint64_t)((uint8_t)bytes[1]) << 48) |
         ((uint64_t)((uint8_t)bytes[2]) << 40) |
         ((uint64_t)((uint8_t)bytes[3]) << 32) |
         ((uint64_t)((uint8_t)bytes[4]) << 24) |
         ((uint64_t)((uint8_t)bytes[5]) << 16) |
         ((uint64_t)((uint8_t)bytes[6]) << 8) | ((uint64_t)((uint8_t)bytes[7]));
}

char *unpack_u64_to_bytes(uint64_t u64) {
  char *data = malloc(8 * sizeof(char));
  if (!data) {
    perror("Could not assign memory.\n");
    return NULL;
  }
  for (int i = 0; i < 8; i++) {
    int shift = 8 * (7 - i);
    data[i] = (char)((u64 >> shift) & 0xFFu);
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

uint64_t renyi_map(uint64_t xj, uint64_t B, unsigned int k) {
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

prints the board struct passed (used for debugging)

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

/*

given a board state, move the piece in 'from' square to 'to' square
handles prey and predators differently

 */
uint64_t make_move(Board *board, Square from, Square to) {
  // retrieve piece to be moved
  Piece p_from = piece_on(board, from);
  Piece p_to = piece_on(board, to);

  // if moving to a predator, warn caller
  if (board->occupancies[1] & (1ULL << to)) {
    return 0;
  }

  // if prey is the one moving
  if (p_from < W_1) {
    // prevent prey from moving to occupied prey squares
    if (board->occupancies[0] & (1ULL << to)) {
      return 0;
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

    // get chaotic path
    uint64_t x = renyi_map(to, renji_B[p_to], renji_params[p_to]);

    return x;
  }
  return 0;
}

uint64_t move_piece(Board *board, Piece p) {
  if (!board->bitboards[p]) {
    return 0;
  }
  // retrieve source square from piece
  Square from = lsb(board->bitboards[p]);

  // placeholder for available moves
  Bitboard moves = 0ULL;

  // check for each direction if square is available
  if (valid_sq(from + NORTH))
    moves |= 1ULL << (from + NORTH);
  if (valid_sq(from + SOUTH))
    moves |= 1ULL << (from + SOUTH);
  // check for warping moves
  if (valid_sq(from + EAST) && !((1ULL << from) & FileHBB))
    moves |= 1ULL << (from + EAST);
  if (valid_sq(from + WEST) && !((1ULL << from) & FileABB))
    moves |= 1ULL << (from + WEST);

  // if piece is a prey
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

      // this is guaranteed to return the renji map value
      return make_move(board, from, to);
    }
    // if no adjacent prey are found, move randomly
  }
  // pop the number of active bits in moves bitboard
  int move_count = popCount(moves);
  int choice = rand() % move_count;   // only choose from available directions
  Square to = nth_bit(moves, choice); // get choice'th lsb

  // once the move is verified to be legal, make the move
  // this call to make move will always return 0
  return make_move(board, from, to);
}

// moves all prey. We do not need to make distinctions as in the predator case
void move_prey(Board *board) {
  for (Piece p = R_1; p < W_1; ++p) {
    move_piece(board, p);
  }
}

/*

central function to the exercise. Takes in a file input, a file output, and a
board. When a captures occurs in the board, the renji map is called, which
returns a value depending on the prey caught. Once all bytes of the original
image have been exhausted, print the resulting image to file output using the
pgm_io library.

 */
void cypher_img(char *file_in, char *file_out, Board *board) {
  int rows, cols;

  // file input
  unsigned char **image;
  image = pgmRead(file_in, &rows, &cols);
  if (image == NULL) {
    printf("Error pgmRead\n");
    exit(1);
  }

  // file output
  unsigned char **image_out = malloc(rows * sizeof(unsigned char *));
  if (!image_out) {
    perror("Error image_out malloc\n");
    // free image if pgmRead allocated it (we assume it did)
    for (int i = 0; i < rows; ++i)
      free(image[i]);
    free(image);
    exit(1);
  }
  for (int r = 0; r < rows; ++r) {
    image_out[r] = malloc(cols * sizeof(unsigned char));
    if (!image_out[r]) {
      perror("malloc image_out[r]");
      for (int k = 0; k < r; ++k)
        free(image_out[k]);
      free(image_out);
      for (int k = 0; k < rows; ++k)
        free(image[k]);
      free(image);
      exit(1);
    }
  }

  uint64_t renji;
  char im_data[8];
  int i = 0;
  int j = 0;
  do {
    move_prey(board);
    renji = move_piece(board, W_1);
    if (renji > 0) {
      for (int k = 0; k < 8; k++) {
        im_data[k] = image[i][j + k];
      }
      // pack 8 separate bytes into 64 contiguous bits
      uint64_t packed_bytes = pack_bytes_to_u64(im_data);

      // xor with renji chaotic map
      uint64_t new_data = packed_bytes ^ renji;
      char *new_bytes = unpack_u64_to_bytes(new_data);
      if (new_bytes) {
        for (int k = 0; k < 8; k++) {
          image_out[i][j + k] = (unsigned char)new_bytes[k];
        }
        free(new_bytes);
      }
      j += 8;
    }
    if (j >= cols) {
      j = 0;
      i++;
    }
    if (i >= rows) {
      break;
    }

    // do predator 2 movement if there are still bytes to encode
    if (i * cols + j < rows * cols - 1) {
      renji = move_piece(board, W_2);
      if (renji > 0) {
        for (int k = 0; k < 8; k++) {
          im_data[k] = image[i][j + k];
        }
        // pack the 8 bytes of image data into a 64bit number
        uint64_t packed_bytes = pack_bytes_to_u64(im_data);

        // apply renji xor
        uint64_t new_data = packed_bytes ^ renji;

        // unpack into 8 bytes
        char *new_bytes = unpack_u64_to_bytes(new_data);
        if (new_bytes) {
          for (int k = 0; k < 8; k++) {
            image_out[i][j + k] = (unsigned char)new_bytes[k];
          }
          free(new_bytes);
        }
        j += 8;
      }
    }
    if (j >= cols) {
      j = 0;
      i++;
    }
    if (i >= rows) {
      break;
    }
  } while (i * cols + j < rows * cols - 1);

  pgmWrite(file_out, rows, cols, image_out, NULL);
  free(image[0]);
  for (int i = 0; i < rows; i++) {
    free(image_out[i]);
  }
  free(image);
  free(image_out);
}

void cypher_wav(const char *file_in, const char *file_out, Board *board) {
  char *wav_data, *wav_out;
  HEADER *header = read_wav(file_in, &wav_data);

  wav_out = malloc(header->data_size * sizeof(char));
  memcpy(wav_out, wav_data, header->data_size);

  uint64_t renji;
  char im_data[8];
  int j = 0;
  do {
    move_prey(board);
    renji = move_piece(board, W_1);
    if (renji > 0) {
      memcpy(im_data, wav_data + j, 8);
      uint64_t packed_bytes = pack_bytes_to_u64(im_data);
      uint64_t new_data = packed_bytes ^ renji;
      char *new_bytes = unpack_u64_to_bytes(new_data);
      if (new_bytes) {
        memcpy(wav_out + j, new_bytes, 8);
        free(new_bytes);
      }
      j += 8;
    }
    // do predator 2 movement if there are still bytes to encode
    if (j < header->data_size) {
      renji = move_piece(board, W_2);
      if (renji > 0) {
        memcpy(im_data, wav_data + j, 8);
        uint64_t packed_bytes = pack_bytes_to_u64(im_data);
        uint64_t new_data = packed_bytes ^ renji;
        char *new_bytes = unpack_u64_to_bytes(new_data);
        if (new_bytes) {
          memcpy(im_data, wav_data + j, 8);
          free(new_bytes);
        }
        j += 8;
      }
    }
  } while (j < header->data_size);

  write_wav(file_out, header->data_size, header, wav_out);
  free(wav_out);
  free(wav_data);
  free(header);
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr,
            "Invalid argument count. Usage: %s <uint: encryption-key> "
            "<file-input (.pgm | .wav)> <file-output>",
            argv[0]);
    exit(1);
  }
  // seed for encryption
  // using different seeds will result in the data being encrypted in another "random" way
  // therefore, if we encrypt using 2020, and apply the same function to the encrypted file,
  // using key 2020 will restore the file, but using 2021 will encrypt it again. To restore a double encryption,
  // we must apply both transformations again (that is, image ^ key1 ^ key2 ^ key2 ^ key1 = image)
  srand(atoi(argv[1]));
  Board *board = malloc(sizeof(Board));
  initialize_board(board);

  const char *dot = strrchr(argv[2], '.');
  if (!dot || dot == argv[2]) {
    fprintf(stderr, "Invalid file extension\n"); // no extension was found
    exit(1);
  }
  const char *extension = dot + 1; // skip the dot

  if (strcmp(extension, "wav") == 0) {
    cypher_wav(argv[2], argv[3], board);
  } else if (strcmp(extension, "pgm") == 0) {
    cypher_img(argv[2], argv[3], board);
  } else {
    fprintf(stderr, "Invalid file extension.\n");
    exit(1);
  }

  free(board);
  return 0;
}
