#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <emmintrin.h>
#include "ktiming.h" 

typedef uint32_t pixel_t;

/* Function prototypes */

pixel_t *allocate_matrix(int n);
void initialize_matrix(pixel_t *src, int n);
void rotate_main(pixel_t *dest, const pixel_t *src, int n);
int check_rotated(const pixel_t *src, const pixel_t *dest, int n);
void print_matrix(const pixel_t *matrix, int n);
uint64_t getAllocationSize(int n);
int getIndex(int x, int y, int d);

/* Extern variables */

//extern int optind;

/* Function definitions */

int main( int argc, char** argv )
{
  int i, j, n, r, optchar, printFlag = 0;
  unsigned int seed = 0;
  clockmark_t time1, time2;
  pixel_t *src, *dest;

  // process command line options
  while( ( optchar = getopt( argc, argv, "s:p" ) ) != -1 ) {
    switch( optchar ) {
      case 's':
        seed = (unsigned int) atoi(optarg);
        printf("Using user-provided seed: %u\n", seed);
        srand(seed);
        break;
      case 'p':
        printFlag = 1;
        break;
      default:
        printf( "Ignoring unrecognized option: %c\n", optchar );
        continue;
    }
  }

  // shift remaining arguments over
  int remaining_args = argc - optind;
  for( i = 1; i <= remaining_args; ++i ) {
    argv[i] = argv[i+optind-1];
  }

  // check to make sure number of arguments is correct
  if (remaining_args != 2) {
    printf("Usage: %s [-p] [-s seed] <length/width_of_image> <num_repeats>\n", argv[0]);
    printf("  -p : print source and result matrices\n");
    printf("  -s : set rand() seed value\n");
    exit(-1); 
  } 

  n = atoi(argv[1]);
  r = atoi(argv[2]);

  // allocate matrices
  src = allocate_matrix(n);
  dest = allocate_matrix(n);

  // initialize src matrix to random numbers
  initialize_matrix(src, n);

  // display source matrix
  if (printFlag) {
    print_matrix(src, n);
  }

  // do rotation r times
  time1 = ktiming_getmark( );
  for (j = 0; j < r; j++)
    rotate_main( dest, src, n );
  time2 = ktiming_getmark( );

  // display result matrix
  if (printFlag) {
    print_matrix(dest, n);
  }

  // check to make sure matrix was rotate correctly
  if (check_rotated(src, dest, n)) {
    printf("Rotated: yes\n");
  } else {
    printf("Rotated: NO!\n");
    exit(-1);
  }

  // report execution time
  float elapsedf = ktiming_diff_sec( &time1, &time2 );
  printf( "Elapsed execution time: %f sec\n", elapsedf );

  return 0;
}

// Allocate memory for matrix
pixel_t *allocate_matrix(int n)
{
  // allocate n*n memory block aligned to multiple-of-64 address
  pixel_t *result = (pixel_t *) _mm_malloc(getAllocationSize(n), 64);
  if (result == NULL) {
    printf("Not enough memory to allocate matrix\n");
    exit(-1);
  }
  return result;
}

// Initialize matrix with random values
void initialize_matrix(pixel_t *src, int n)
{
  int i, j;
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      src[getIndex(i, j, n)] = rand() % 100;
    }
  }
}

// Check to see if matrix was rotated properly
int check_rotated(const pixel_t *src, const pixel_t *dest, int n)
{ 
  int i, j;
  for (i = 0; i < n; i++) { 
    for (j = 0; j < n; j++) { 
      if (dest[getIndex(n - j - 1, i, n)] != src[getIndex(i, j, n)]) {
        return 0;
      }
    }
  }
  return 1;
}

// Print matrix to console
void print_matrix(const pixel_t *matrix, int n)
{ 
  int i, j;
  for (i = 0; i < n; i++) { 
    for (j = 0; j < n; j++) {
      printf("%d ", matrix[getIndex(i, j, n)]);
    }
    printf("\n");
  }
  printf("\n");
}
