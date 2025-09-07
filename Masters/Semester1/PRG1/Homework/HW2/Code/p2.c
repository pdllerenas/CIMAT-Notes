#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define N 10
#define M 10

#define x_0 0
#define y_0 0

char board[N][M];

void initialize_board(char board[N][M]) {
  char *p, *END = &board[0][0] + M * N;
  p = &board[0][0];
  int i;

  for (p = board[0], i = 0; p < END; p++, i++) {
    *p = '.';
  }
}

void print_board() {
  printf("\033[H");
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      printf("%c ", board[i][j]);
    }
    printf("\n");
    fflush(stdout);
  }
}

int main() {
  initialize_board(board);
  printf("\033[2J");

  srand(time(NULL));
  char *current = &board[x_0][y_0];
  unsigned char letter = '.' + 1; // avoid '.' char
  *current = letter++; // set first tile to first character, then increment char
                       // for next usage

  char *POS_0M = &board[0][M], *POS_N0 = &board[N][0]; // helper pointers
  short int iter = 0;                                  // iteration count
  // pos_x is used to keep track of column position in matrix of current block.
  // We use this to check if we can go left or right
  short int pos_x = x_0;
  unsigned char direction, blocked = 0;
  /*
  we use 4 bits (half a byte) to describe directions:
  0000 0001 = up
  0000 0010 = down
  0000 0100 = left
  0000 1000 = right

  blocked represents the directions that have been proven to be blocked
 */

  while (iter < 1000) {
    if (blocked == 15) {
      break; // if blocked == 00001111 (all directions are blocked), exit loop
    }
    iter++;
    direction = 1 << (rand() % 4);
    if (direction & blocked) { // if direction randomly chosen has at least one
                               // bit in common with blocked choices, try again
      continue;
    }
    switch (direction) {
    case 1:
      if (current < POS_0M ||
          *(current - N) != '.') // if current position is in first row or north
                                 // character is not a '.', skip
      {
        blocked |= direction; // add blocked direction to blocked variable
        break;
      } else {
        current -= N; // go back N * sizeof(char) = N bytes. This places us +1
                      // position north
        *current = letter++; // set the new current position to letter, then
                             // increment the letter.

        // after moving, we block the square we come from. For
        // example, if we move north (this case), then we block
        // south, which is 0000 0010 (direction << 1)
        blocked = direction << 1;
        print_board();
        usleep(200000);
        break;
      }
    case 2:
      if (current > POS_N0 || *(current + N) != '.') {
        blocked |= direction;
        break;
      } else {
        current += N; // go 1 block south in matrix
        *current = letter++;
        blocked = direction >> 1; // since we moved south, blocked is 0000 0001
        print_board();
        usleep(200000);
        break;
      }
    case 4:
      if (pos_x == 0 || *(current - 1) != '.') {
        blocked |= direction;
        break;
      } else {
        current--;                // go left
        *current = letter++;      // set current position to letter
        blocked = direction << 1; // since we moved left, blocked is 0000 1000
        pos_x--;                  // update column position
        print_board();
        usleep(200000);
        break;
      }
    case 8:
      if (pos_x == M - 1 || *(current + 1) != '.') {
        blocked |= direction;
        break;
      } else {
        current++;                // go right
        *current = letter++;      // set current char to letter, then update
        blocked = direction >> 1; // since we moved right, blocked is 0000 01000
        pos_x++;                  // update column position
        print_board();
        usleep(200000);
        break;
      }
    default:
      fprintf(stderr, "Invalid direction.\n");
      exit(1);
    }
  }
}
