#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <xmmintrin.h>

/* Typedefs */

typedef uint32_t data_t;

/* Function prototypes */

/* Macros */

#define RADIX_BITS 4
#define BUCKETS (1 << RADIX_BITS)
#define MAX_ARRAY_SIZE 100000

/* Globals */

/* For simplicity, we will statically allocate the radix sort queues here.  
 * Note that if we make this array too big, the program will fail to load when
 * started. */
data_t queues[BUCKETS][MAX_ARRAY_SIZE];

/* Function definitions */

/* Radix sort */
void sort(data_t *left, data_t *right) 
{
  /* Use this to keep track of the tail of each of the queues */
  int queue_tails[BUCKETS] = {0}; 

  /* Fill in this function... */

  /* Prefetch data with this function: 
    _mm_prefetch((const char *) <address>, _MM_HINT_T0); */
}
