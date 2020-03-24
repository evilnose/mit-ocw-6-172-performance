#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* Typedefs */

typedef uint32_t data_t;

/* Function prototypes */

inline data_t *partition(data_t *left, data_t *right);
inline void swap(data_t *a, data_t *b);

/* Function definitions */

/* Basic quick sort */
void sort(data_t *left, data_t *right) 
{
  data_t *p;

  if (right - left < 1) {
    return;
  }

  p = partition(left, right);
  sort(left, p - 1);
  sort(p + 1, right);
}

/* A fast partition function.  Partitions the array into two parts so that all
 * values from left to the return pointer are smaller than the values from the
 * return pointer + 1 to right.  
 */
inline data_t *partition(data_t *left, data_t *right) 
{ 
  data_t *pivot_ptr = left; 
  data_t pivot = *pivot_ptr;		/* Choose first element as pivot */

  left++;

  while (left < right) {
    while (left < right && *left <= pivot) {
      left++;
    }
    while (left < right && *right > pivot) {
      right--;
    }
    if (left < right) {
      swap(left, right);
    }
  }

  if (pivot > *left) {
    swap(pivot_ptr, left);
    return left;
  } else {
    swap(pivot_ptr, left - 1);
    return left - 1;
  }
}

inline void swap(data_t *a, data_t *b)
{
  data_t tmp = *a;
  *a = *b;
  *b = tmp;
}

