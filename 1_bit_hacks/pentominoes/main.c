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
 * main.c:
 *
 * This file runs your code, timing its execution and printing out the result.
 *
 * You should not modify this code; it will be overwritten when we run your
 * code.
 *
 * If you find any bugs in this code, you should contact the 6.172 staff.
 *
 **/

#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ktiming.h"
#include "pentominoes.h"


#define ON_BOARD(x, y) ( ( (x) >= 0) && ((x) < 8) && ( (y) >= 0) && ( (y) < 8) )

/* Forward declarations. */
static int run_test_suite(int start_idx);

// A test case is a function that takes no arguments and returns no arguments.
typedef void (*ptest_case)(void);

extern ptest_case your_test_cases[];

static int show_usec = 0;
static int print_solns = 0;
static int solns_limit = 1;
static unsigned long num_solns = 0;

/**
 * Handles solutions found by the solver. Return TRUE to exit, FALSE to continue running.
 **/
static bool got_solution(board_t *board) {
  if (print_solns)
    print_board(board);
  num_solns++;
  return (solns_limit > 0 && num_solns >= solns_limit);
}

static void print_usage(const char *argv_0) {
    printf("Usage: %s "
           "-b \"block1_x,block1_y block2_x,block2_y "
           "block3_x,block3_y, block4_x,block4_y\"\n", argv_0);
    printf("Other Parameters:\n"
           "\t-u: Shows timestamps in microseconds\n"
           "\t-a: Find all solutions (default: exits after one solution)\n"
           "\t-n 1000: Find first 1000 solutions.\n"
           "\t-p: Prints out each solution (default: only output number of solutions)\n"
           "\t-t 0: Run test suite, starting from the first test\n");
}

int main(int argc, char** argv) {
  char optchar;
  clockmark_t time1, time2;

  // Blocked positions
  int ex1=-1, ey1=-1, ex2=-1, ey2=-1, ex3=-1, ey3=-1, ex4=-1, ey4=-1;
  opterr = 0;

  while ((optchar = getopt(argc, argv, "b:n:upat:")) != -1) {
    switch (optchar) {
      case 'b': // Blocked positions
        sscanf(optarg, "%d,%d %d,%d %d,%d %d,%d", &ex1, &ey1, &ex2, &ey2,
                                                  &ex3, &ey3, &ex4, &ey4);
        break;
      case 'u':
        show_usec = 1;
        break;
      case 'a':
        solns_limit = 0;
        break;
      case 'n':
        solns_limit = atoi(optarg);
        break;
      case 'p':
        print_solns = 1;
        break;
      case 't':
        return run_test_suite(atoi(optarg));
        break;
      default:
        printf("Unrecognized option: %c\n", optchar);
        print_usage(argv[0]);
        exit(-1);
        break;
    }
  }
  if(!(ON_BOARD(ex1, ey1) && ON_BOARD(ex2, ey2) &&
       ON_BOARD(ex3, ey3) && ON_BOARD(ex4, ey4)))
  {
    fprintf(stderr, "Some of the coordinates specified are invalid.\n");
    print_usage(argv[0]);
    exit(-1);
  }

  board_t *board = board_new_frompoints(ex1, ey1, ex2, ey2, ex3, ey3, ex4, ey4);
  
  fprintf(stderr, "Running solve()...\n");

  /** WARNING! DO NOT CHANGE PRINT STATEMENTS BELOW THIS LINE! **/
  printf("---- RESULTS ----\n");

  time1 = ktiming_getmark();
  bool r = solve(board, got_solution);
  time2 = ktiming_getmark();

  printf("Solutions found: %lu\n", num_solns);
  printf("---- END RESULTS ----\n");

  if (r) {
    fprintf(stderr, "note: concluded before all solutions were found\n");
  }

  uint64_t elapsed = ktiming_diff_usec(&time1, &time2);
  float elapsedf = ktiming_diff_sec(&time1, &time2);

  if (show_usec) {
    printf("Elapsed execution time: %llu usec\n", (long long unsigned) elapsed);
  } else {
    printf("Elapsed execution time: %f sec\n", elapsedf);
  }

  board_free(board);
  exit(0);
}


/**
 * Run the user's test suite, starting from start_idx (zero-indexed)
 **/
static int run_test_suite(int start_idx) {
  while(your_test_cases[start_idx])
  {
    fprintf(stderr, "Running test #%d...\n", start_idx);
    (*your_test_cases[start_idx])();
    start_idx++;
  }
  fprintf(stderr, "Done testing.\n");
  return 0;
}

/**
 * Prints the board. The formatted output must remain of exactly
 * the same format for testing.
 **/
void print_board(board_t *board) {
  int i,j;

  printf("--------\n");

  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      if (board_get_square(board, i, j)) {
        putc(board_get_square(board, i, j), stdout);
      } else {
        putc('.', stdout);
      }
    }
    printf("\n");
  }
  printf("--------\n");
}
