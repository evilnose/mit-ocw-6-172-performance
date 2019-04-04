#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <math.h>
#include <assert.h>

#include "matrix_multiply.h"

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


/*
 * Allocates a row-by-cols matrix and returns it
 */
matrix* make_matrix(int rows, int cols)
{
  matrix* new_matrix = malloc(sizeof(matrix));

  // Set the number of rows and columns
  new_matrix->rows = rows;
  new_matrix->cols = cols;

  // Allocate a buffer big enough to hold the matrix.
  new_matrix->values = (int**)malloc(sizeof(int*) * rows);
  int i;
  for (i = 0; i < rows; i++) {
    new_matrix->values[i] = (int*)malloc(sizeof(int) * cols);
  }

  return new_matrix; 
}

/*
 * Frees an allocated matrix
 */
void free_matrix(matrix* m)
{
  int i;
  for (i = 0; i < m->rows; i++) {
    free(m->values[i]);
  }
  free(m->values);
  free(m);
}

/*
 * Print Matrix
 */
void print_matrix(const matrix* m)
{
  int i, j;
  printf("------------\n");
  for (i = 0; i < m->rows; i++) {
    for (j = 0; j < m->cols; j++) {
      printf("  %d  ", m->values[i][j]);
    }
    printf("\n");
  }
  printf("------------\n");
}


/**
 * Multiply matrix A*B, store result in C.
 */
int matrix_multiply_run(const matrix* A, const matrix* B, matrix* C)
{
  int i, j, k;
  assert(A->rows == B->cols);
  assert(A->cols == B->rows);
  assert(A->rows == C->rows);
  assert(B->cols == C->cols);
  for (i = 0; i < A->rows; i++) {
    for (k = 0; k < A->cols; k++) {
      for (j = 0; j < B->cols; j++) {
        C->values[i][j] += A->values[i][k] * B->values[k][j];
      }
    }
  }
  return 0;
}
