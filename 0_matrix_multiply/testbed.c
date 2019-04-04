/**
 Copyright (c) 2010 by 6.172 Staff <6.172-staff@mit.edu>

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
**/


/**
 * testbed.c:
 * 
 * This file runs your code, timing its execution and printing out the result.
 *
 * You should not modify this code; it will be overwritten when we run your
 * code.
 *
 * If you find any bugs in this code, you should contact the 6.172 staff.
 *
 **/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "ktiming.h" 
#include "matrix_multiply.h"


int main(int argc, char** argv)
{
  int optchar;
  int show_usec = 0;
  int should_print = 0;
  int i, j;
  matrix* A;
  matrix* B;
  matrix* C;

  opterr = 0;

  while ((optchar = getopt(argc, argv, "up")) != -1) {
    switch (optchar) {
      case 'u':
        show_usec = 1;
        break;
      case 'p':
        should_print = 1;
        break;
      default:
        printf("Ignoring unrecognized option: %c\n", optchar);
        continue;
    }
  }
  fprintf(stderr, "Setup\n");
  A = make_matrix(1000, 1000);
  B = make_matrix(1000, 1000);
  C = make_matrix(1000, 1000);

  for (i = 0; i < A->rows; i++) {
    for (j = 0; j < A->cols; j++) {
      A->values[i][j] = A->rows * i + j + 1;
    }
  }
  for (i = 0; i < B->rows; i++) {
    for (j = 0; j < B->cols; j++) {
      B->values[i][j] = B->rows * i + j + 1;
    }
  }

  if (should_print) {
    printf("Matrix A: \n");
    print_matrix(A);
  }

  if (should_print) {
    printf("Matrix B: \n");
    print_matrix(B);
  }

  fprintf(stderr, "Running matrix_multiply_run()...\n");

  clockmark_t time1 = ktiming_getmark();
  matrix_multiply_run(A, B, C);
  clockmark_t time2 = ktiming_getmark();

  uint64_t elapsed = ktiming_diff_usec(&time1, &time2);
  float elapsedf = ktiming_diff_sec(&time1, &time2);

  /** WARNING! DO NOT CHANGE PRINT STATEMENTS BELOW THIS LINE! **/
  if (should_print) {
    printf("---- RESULTS ----\n");
    printf("Result: \n");
    print_matrix(C);
    printf("---- END RESULTS ----\n");
  }
  if (show_usec) {
    printf("Elapsed execution time: %llu usec\n", (long long unsigned)elapsed);
  } else {
    printf("Elapsed execution time: %f sec\n", elapsedf);
  }

  return 0;
}
