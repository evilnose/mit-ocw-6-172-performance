/**
 * Copyright (c) 2010 6.172 Staff
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 **/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "pentominoes.h"

typedef struct point_t {
  int8_t x;
  int8_t y;
} point_t;

struct pentomino_t {
  uint8_t letter;
  point_t points[5];
};

void board_set_square(board_t *board, int x, int y, char val);


struct stboard {
  uint8_t grid[8][8];
};

const pentomino_t pieces [] = {
  {'I', {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}}},
  {'I', {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}}},
  {'X', {{0, 0}, {1, 7}, {1, 0}, {1, 1}, {2, 0}}},
  {'Z', {{0, 0}, {0, 1}, {1, 0}, {2, 7}, {2, 0}}},
  {'Z', {{0, 0}, {1, 0}, {1, 1}, {1, 2}, {2, 2}}},
  {'Z', {{0, 0}, {0, 1}, {1, 1}, {2, 1}, {2, 2}}},
  {'Z', {{0, 0}, {1, 6}, {1, 7}, {1, 0}, {2, 6}}},
  {'V', {{0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2}}},
  {'V', {{0, 0}, {0, 1}, {0, 2}, {1, 0}, {2, 0}}},
  {'V', {{0, 0}, {1, 0}, {2, 6}, {2, 7}, {2, 0}}},
  {'V', {{0, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 2}}},
  {'T', {{0, 0}, {0, 1}, {0, 2}, {1, 1}, {2, 1}}},
  {'T', {{0, 0}, {1, 6}, {1, 7}, {1, 0}, {2, 0}}},
  {'T', {{0, 0}, {1, 0}, {2, 7}, {2, 0}, {2, 1}}},
  {'T', {{0, 0}, {1, 0}, {1, 1}, {1, 2}, {2, 0}}},
  {'W', {{0, 0}, {1, 0}, {1, 1}, {2, 1}, {2, 2}}},
  {'W', {{0, 0}, {1, 7}, {1, 0}, {2, 6}, {2, 7}}},
  {'W', {{0, 0}, {0, 1}, {1, 1}, {1, 2}, {2, 2}}},
  {'W', {{0, 0}, {0, 1}, {1, 7}, {1, 0}, {2, 7}}},
  {'U', {{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 2}}},
  {'U', {{0, 0}, {0, 1}, {1, 1}, {2, 0}, {2, 1}}},
  {'U', {{0, 0}, {0, 2}, {1, 0}, {1, 1}, {1, 2}}},
  {'U', {{0, 0}, {0, 1}, {1, 0}, {2, 0}, {2, 1}}},
  {'L', {{0, 0}, {1, 0}, {1, 1}, {1, 2}, {1, 3}}},
  {'L', {{0, 0}, {1, 0}, {2, 0}, {3, 7}, {3, 0}}},
  {'L', {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 3}}},
  {'L', {{0, 0}, {0, 1}, {1, 0}, {2, 0}, {3, 0}}},
  {'L', {{0, 0}, {0, 1}, {1, 1}, {2, 1}, {3, 1}}},
  {'L', {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 0}}},
  {'L', {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {3, 1}}},
  {'L', {{0, 0}, {1, 5}, {1, 6}, {1, 7}, {1, 0}}},
  {'N', {{0, 0}, {0, 1}, {1, 6}, {1, 7}, {1, 0}}},
  {'N', {{0, 0}, {1, 0}, {1, 1}, {2, 1}, {3, 1}}},
  {'N', {{0, 0}, {0, 1}, {0, 2}, {1, 7}, {1, 0}}},
  {'N', {{0, 0}, {1, 0}, {2, 0}, {2, 1}, {3, 1}}},
  {'N', {{0, 0}, {0, 1}, {1, 1}, {1, 2}, {1, 3}}},
  {'N', {{0, 0}, {1, 0}, {2, 7}, {2, 0}, {3, 7}}},
  {'N', {{0, 0}, {0, 1}, {0, 2}, {1, 2}, {1, 3}}},
  {'N', {{0, 0}, {1, 7}, {1, 0}, {2, 7}, {3, 7}}},
  {'Y', {{0, 0}, {1, 6}, {1, 7}, {1, 0}, {1, 1}}},
  {'Y', {{0, 0}, {1, 7}, {1, 0}, {2, 0}, {3, 0}}},
  {'Y', {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 1}}},
  {'Y', {{0, 0}, {1, 0}, {2, 0}, {2, 1}, {3, 0}}},
  {'Y', {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 2}}},
  {'Y', {{0, 0}, {1, 0}, {1, 1}, {2, 0}, {3, 0}}},
  {'Y', {{0, 0}, {1, 7}, {1, 0}, {1, 1}, {1, 2}}},
  {'Y', {{0, 0}, {1, 0}, {2, 7}, {2, 0}, {3, 0}}},
  {'F', {{0, 0}, {1, 7}, {1, 0}, {1, 1}, {2, 1}}},
  {'F', {{0, 0}, {0, 1}, {1, 7}, {1, 0}, {2, 0}}},
  {'F', {{0, 0}, {1, 0}, {1, 1}, {1, 2}, {2, 1}}},
  {'F', {{0, 0}, {1, 0}, {1, 1}, {2, 7}, {2, 0}}},
  {'F', {{0, 0}, {1, 6}, {1, 7}, {1, 0}, {2, 7}}},
  {'F', {{0, 0}, {0, 1}, {1, 1}, {1, 2}, {2, 1}}},
  {'F', {{0, 0}, {1, 7}, {1, 0}, {1, 1}, {2, 7}}},
  {'F', {{0, 0}, {1, 7}, {1, 0}, {2, 0}, {2, 1}}},
  {'P', {{0, 0}, {0, 1}, {1, 0}, {1, 1}, {2, 1}}},
  {'P', {{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}}},
  {'P', {{0, 0}, {1, 0}, {1, 1}, {2, 0}, {2, 1}}},
  {'P', {{0, 0}, {0, 1}, {1, 7}, {1, 0}, {1, 1}}},
  {'P', {{0, 0}, {0, 1}, {1, 0}, {1, 1}, {1, 2}}},
  {'P', {{0, 0}, {1, 7}, {1, 0}, {2, 7}, {2, 0}}},
  {'P', {{0, 0}, {0, 1}, {0, 2}, {1, 1}, {1, 2}}},
  {'P', {{0, 0}, {0, 1}, {1, 0}, {1, 1}, {2, 0}}},
  {0}
};

board_t *board_new(void)
{
  return calloc(1, sizeof(board_t));
}

board_t *board_new_frompoints(uint8_t ex1, uint8_t ey1,
                              uint8_t ex2, uint8_t ey2,
                              uint8_t ex3, uint8_t ey3,
                              uint8_t ex4, uint8_t ey4)
{
  board_t *ret = board_new();
  if (ret == NULL)
    return NULL;
  board_set_square(ret, ex1, ey1, '#');
  board_set_square(ret, ex2, ey2, '#');
  board_set_square(ret, ex3, ey3, '#');
  board_set_square(ret, ex4, ey4, '#');
  return ret;
}

void board_free(board_t *board) {
  free(board);
}

char board_get_square(board_t *board, int x, int y) {
  assert(x < 8 && y < 8 && x >= 0 && y >= 0);
  return (char) board->grid[x][y];
}

void board_set_square(board_t *board, int x, int y, char val) {
  assert(x < 8 && y < 8 && x >= 0 && y >= 0);
  board->grid[x][y] = (uint8_t) val;
}

void fill_piece(board_t *board, const pentomino_t *piece_description,
                int x, int y) {
  int i;
  assert(board_get_square(board, x, y) == 0);
  for (i = 0; i < 5; i++) {
    int pos_x, pos_y;
    pos_x = (x + piece_description->points[i].x) % 8;
    pos_y = (y + piece_description->points[i].y) % 8;

    // Make sure position is valid and unoccupied.
    assert(pos_x >= 0 && pos_x < 8);
    assert(pos_y >= 0 && pos_y < 8);
    assert(board_get_square(board, pos_x, pos_y) == 0);
    board_set_square(board, pos_x, pos_y, piece_description->letter);
  }
}

int can_fill_piece(board_t *board, const pentomino_t *piece_description,
                   int x, int y) {
  int i;
  assert(0 <= x && x < 8 && 0 <= y && y < 8);
  for (i = 0; i < 5; i++) {
    int pos_x, pos_y;
    pos_x = (x + piece_description->points[i].x) % 8;
    pos_y = (y + piece_description->points[i].y) % 8;

    // Make sure position is valid and unoccupied.
    assert(pos_x >= 0 && pos_x < 8);
    assert(pos_y >= 0 && pos_y < 8);
    if (!(board_get_square(board, pos_x, pos_y) == 0))
      return 0;
  }
  return 1;
}

int remove_piece(board_t *board, const pentomino_t *piece_description) {
  int count = 0;
  int i, j;
  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      if (board_get_square(board, i, j) == piece_description->letter) {
        // We found a fragment of the piece. Remove it.
        board_set_square(board, i, j, 0);
        count++;
      }
    }
  }
  // We should've removed either a complete piece, or nothing.
  assert(count == 0 || count == 5);
  return count;
}

/**
 * Returns 1 if puzzle is solved.
 **/
static int is_solved(board_t *board) {
  int i=0, j=0;
  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      if (board_get_square(board, i, j) == 0) {
        return 0;
      }
    }
  }
  return 1;
}

/**
 * Find the first empty location on the board, and set *x and *y to that.
 **/
static void find_first_empty(board_t *board, int *x, int *y) {
  int next_x = 0;
  int next_y = 0;

  while (next_x < 8 && board_get_square(board, next_x, next_y) != 0) {
    next_y++;
    if (next_y == 8) {
      next_y = 0;
      next_x++;
    }
  }

  *x = next_x;
  *y = next_y;
}

/**
 * Recursive solver for row,col. Return true if the search should be
 * stopped, false otherwise.
 **/
static int solve_internal(uint8_t *used, board_t *board, solution_handler_t cb,
                          unsigned long *pieces_filled,
                          unsigned long *num_solns) {
  int p;
  int x = 0;
  int y = 0;

  if (is_solved(board)) {
    // Solved!  Save solution.
    (*num_solns)++;
    if (cb)  // Call custom solution handler if not NULL
      return (*cb)(board);
    return 0;
  }

  find_first_empty(board, &x, &y);

  // This spot should be empty, and nothing above or to the left of us should
  // be empty.
  assert(board_get_square(board, x, y) == 0);
  assert(x == 0 || board_get_square(board, x - 1, y) != 0);
  assert(y == 0 || board_get_square(board, x, y - 1) != 0);

  // The last element of the pieces array is zero-initialized, so we stop the
  // loop when the letter is zero.
  for (p = 0; pieces[p].letter != 0; p++) {
    // Pieces can only be used once.
    if (used[pieces[p].letter]) {
      continue;
    }

    // Check if we can place the lexicographically least square of this piece
    // at the coordinates (x, y).
    // Special case for x == 0 or y == 0: try harder to fit a piece in.  We
    // can't just test the pentomino's lexicographically least square.
    int i;
    for (i = 0; i == 0 || (i < 5 && (x == 0 || y == 0)); i++) {
      int px = (x - pieces[p].points[i].x + 8) % 8;
      int py = (y - pieces[p].points[i].y + 8) % 8;
      if (can_fill_piece(board, &pieces[p], px, py)) {
        fill_piece(board, &pieces[p], px, py);

        (*pieces_filled)++;
        if ((*pieces_filled) % (1024 * 1024) == 0) {
          fprintf(stdout,
                  "\33[2K\rpieces filled: %010ld, solutions found: %08lu\n",
                  *pieces_filled, *num_solns);
        }

        // Mark this letter piece as being used.
        used[pieces[p].letter] = pieces[p].letter;

        // Find next empty space to use
        int r = solve_internal(used, board, cb, pieces_filled, num_solns);
        if (r)
          return r;

        // Backtrack
        remove_piece(board, &pieces[p]);
        used[pieces[p].letter] = 0;
      }
    }
  }
  return 0;
}

bool solve(board_t *board, solution_handler_t cb) {
  uint8_t used[256];
  memset(used,0,256);
  unsigned long pieces_filled = 0;
  unsigned long num_solns = 0;
  return solve_internal(used, board, cb, &pieces_filled, &num_solns);
}
