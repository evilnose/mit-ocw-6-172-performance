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

/**
 * test_cases.c: A simple testing framework. Please write your test cases in
 * this file.
 *
 * This framework allows us to automatically run your test suite against other
 * groups' submissions.
 *
 * We will copy your tests/ directory (this directory) to each student
 * repository to run the tests, so please do not add any extra .c files in
 * here! Data files (e.g. input) are fine, of course!
 **/


#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ktiming.h"
#include "pentominoes.h"

// A test case is a function that takes no arguments and returns no arguments.
typedef void (*ptest_case)(void);

/**
 * Function declarations:
 *
 * You should declare each of your test cases as functions here, as well as
 * other functions.
 *
 * Test names that start with ref_ are "reference" test cases that we provide
 * you. Their results will be ignored when we run your test suite, so feel free
 * to leave them in or take them out -- just don't put your own work in there!
 * Also, don't name any of your test cases starting with ref_ either.
 *
 * Your test names should be something descriptive.
 **/
static void ref_easy_board_find_solution(void);
static void ref_test_invalid_unfilled(void);
static void ref_test_invalid_letter_used_twice(void);
static void ref_test_valid(void);

static int validate_board(board_t *board);
static int validate_char_board(const char *board_ptr);

/**
 * List all of the test cases you'd like for us to run.
 **/
ptest_case your_test_cases[] =
{
  ref_easy_board_find_solution,
  ref_test_invalid_unfilled,
  ref_test_invalid_letter_used_twice,
  ref_test_valid,
  // ADD YOUR TEST CASES HERE
  NULL // This marks the end of all test cases. Don't change this!
};



// Call TEST_PASS() from your test cases to mark a test as successful
//
#define TEST_PASS() TEST_PASS_WITH_NAME(__func__, __LINE__)

#define TEST_PASS_WITH_NAME(name, line) \
    fprintf(stderr, " --> %s at line %d: PASS\n", (name), (line))

// Call TEST_FAIL from your test cases to mark a test as failed. TEST_FAIL
// should print a meaningful message with the reason that the test failed.
//
// Calling it is just like calling printf().
#define TEST_FAIL(failure_msg, args...) \
    TEST_FAIL_WITH_NAME(__func__, __LINE__, failure_msg, ##args)

#define TEST_FAIL_WITH_NAME(name, line, failure_msg, args...) do { \
    fprintf(stderr, " --> %s at line %d: FAIL\n    Reason:", (name), (line)); \
    fprintf(stderr, (failure_msg), ## args); fprintf(stderr, "\n"); \
} while (0)


#define TEST_ASSERT(condition, fail_args...) do { \
  if (condition) { \
    TEST_PASS(); \
  } else { \
    TEST_FAIL(fail_args); \
  } \
} while(0)


int num_solutions = 0;

static bool ref_easy_board_find_solution_handler(board_t *board)
{
  // The fact that we got a solution is awesome. Increment counter and stop
  // looking for solutions.
  if (validate_board(board))
    num_solutions++;
  else
    printf("The solution you found was invalid!\n");
  return true;
}

/**
 * Sets up a board with easy obstacles; Your solver should find a solution to
 * it relatively quickly.
 **/
static void ref_easy_board_find_solution(void)
{
  board_t *board = board_new_frompoints(0,0,  0,1, 0,2,  0,3);

  // Call ref_easy_board_find_solution_handler() when a solution is found.
  num_solutions = 0;
  // Run!
  print_board(board);
  solve(board, ref_easy_board_find_solution_handler);
  fprintf(stdout, "\n");  // To avoid printing on the same line.
  board_free(board);

  // Pass the test if the user finds one solution.
  if (num_solutions > 0)
    TEST_PASS();
  else
    TEST_FAIL("Failed to find at least one solution!");
}

/**
 * Tests that the validator rejects boards that aren't filled.
 **/
static void ref_test_invalid_unfilled(void)
{
  const char *fake_board =
      "#......#"
      "........"
      "........"
      "........"
      "........"
      "........"
      "........"
      "#......#";
  TEST_ASSERT(!validate_char_board(fake_board),
              "Empty board passed validation!");
}

/**
 * Tests that the validator rejects boards using the same letter twice.
 **/
static void ref_test_invalid_letter_used_twice(void)
{
  const char *fake_board =
      "IIIIIIII"
      "IIIIIIII"
      "IIIIIIII"
      "III##III"
      "III##III"
      "IIIIIIII"
      "IIIIIIII"
      "IIIIIIII";
  TEST_ASSERT(!validate_char_board(fake_board),
              "One letter board passed validation!");
}

/**
 * Tests that the validator rejects boards using the same letter twice.
 **/
static void ref_test_valid(void)
{
  const char *fake_board =
      "#IIIIIX#"
      "ZVVVWXXX"
      "ZZZVWWXT"
      "UUZVLWWT"
      "TUPPLFTT"
      "UUPPLFFF"
      "NNPLLYFN"
      "#NNYYYY#";
  TEST_ASSERT(validate_char_board(fake_board),
              "Valid board failed validation!");
}

/* Yes, we have to duplicate these here, to allow students to change the
 * underlying representation for the piece descriptions.
 */
typedef struct point_t {
  int8_t x;
  int8_t y;
} point_t;

struct pentomino_t {
  uint8_t letter;
  point_t points[5];
};

static const pentomino_t pieces[] = {
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


static char used[256];


/** Return 1 if board is valid, 0 if invalid **/
static int validate_board(board_t *orig_board)
{
  char board[8][8];
  // Make a clean copy of the board that we can modify.
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      board[i][j] = board_get_square(orig_board, i, j);
    }
  }

  return validate_char_board(&board[0][0]);
}

static int validate_char_board(const char *board_ptr) {
  char board[8][8];
  // Make a clean copy of the board that we can modify.
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      board[i][j] = *board_ptr++;
    }
  }

  int empty_count = 0;
  int filled_count = 0;
  // (1) Count up the # of empty spots and the # of pre-filled spots.
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (board[i][j] == 0) {
        empty_count++;
      }
      if (board[i][j] == '#') {
        filled_count++;
      }
    }
  }

  if (filled_count != 4 || empty_count != 0) {
    // Board isn't all occupied, or the # of filled square isn't 4.
    return 0;
  }

  memset(used, 0, 256);

  // Erase letters that actually form the right shape.
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      uint8_t letter = board[x][y];
      if (letter == 0) {
        continue;
      } else if (letter == '#') {
        board[x][y] = 0; // Erase # too, so validation is easy.
        continue;
      }
      if (used[letter])
        return 0;

      for (int p = 0; pieces[p].letter != 0; p++) {
        if (letter != pieces[p].letter)
          continue;

        int fits = 1;
        for (int i = 0; i < 5; i++) {
          int xi = (x + pieces[p].points[i].x) % 8;
          int yi = (y + pieces[p].points[i].y) % 8;
          if (letter != board[xi][yi]) {
            fits = 0;
            break;
          }
        }

        if (fits) {
          for (int i = 0; i < 5; i++) {
            int xi = (x + pieces[p].points[i].x) % 8;
            int yi = (y + pieces[p].points[i].y) % 8;
            board[xi][yi] = 0;
          }
          used[letter] = 1;
          break;
        }
      }
    }
  }

  // Check that the board is empty now.
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      if (board[x][y] != 0) {
        return 0;
      }
    }
  }

  return 1;
}
