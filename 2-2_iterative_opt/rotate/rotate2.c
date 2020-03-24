#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#if 0
1.2 Answer
The issue is that for size 512, when I am writing to each column of
the output matrix, they all map to the same cache set. Need to pretend
that the row length is something not a multiple of 2 so that they map to
different sets as in 511/513.

For larger matrix sizes like 2048, the cache is full after 2 rows are
rotated and so there will be capacity misses and the effect of conflict
misses become diminished, i.e. when writing to the next column of the 
output matrix the previous column has already been evicted from the cache
due to over-filling.
#endif

/* Typedefs */

typedef uint32_t pixel_t;

/* Function declarations */

int getIndex(int x, int y, int d);

/* Function definitions */

// This is the function that does the actual rotation.
// Only time spent in this function is reported by the testbed.
void rotate_main(pixel_t *dest, const pixel_t *src, int n) 
{
  int i, j;
	// this is faster perhaps because caching writes results in
	// more performance improvement than caching reads (because
	// this faster version allows for better caching when 
	// writing to the output matrix)
	for (j = 0; j < n; j++) {
		for (i = 0; i < n; i++) {
      dest[getIndex(n - 1 - j, i, n)] = src[getIndex(i, j, n)];
    }
  }
}

/* Helper function definitions

   These functions must be modified if you make any changes
   to the data layout of the matrices being rotated. They are
   helper functions used during allocation, initialization,
   printing, and correctness checking. */

// This function returns the number of bytes needed to store
// an nxn matrix.
uint64_t getAllocationSize(int n)
{
	n = (n & ~1) + 1;
  return n * n * sizeof(pixel_t);
}

// This function translates a 2D index pair (x, y) into a linear array index.
// Assumes matrix is stored in row-major order and the row stride is d.
int getIndex(int x, int y, int d)
{
	d = (d & ~1) + 1;
  return x * d + y;
}

