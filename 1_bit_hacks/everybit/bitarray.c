/*  Copyright (c) 2010 6.172 Staff

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
*/

/* Implements the ADT specified in bitarray.h as a packed array of bits; a
 * bitarray containing bit_sz bits will consume roughly bit_sz/8 bytes of
 * memory. */

#include <assert.h>
#include <stdio.h>

#include "bitarray.h"

#define BLOCK_SZ 64

typedef signed long long buf_t;

#define MSB_MASK ((buf_t) 1) << (BLOCK_SZ - 1)

#define DEBRUIJN 0x022fdd63cc95386d // the 4061955.

#define preserve_only(x, n) ((x) & ~((buf_t) -1 << (n)))

#define B2 0x0f0f0f0f0f0f0f0f
#define B1 0x3333333333333333
#define B0 0x5555555555555555

static const unsigned int debruijn_table[64] =
{
    0,  1,  2, 53,  3,  7, 54, 27,
    4, 38, 41,  8, 34, 55, 48, 28,
    62,  5, 39, 46, 44, 42, 22,  9,
    24, 35, 59, 56, 49, 18, 29, 11,
    63, 52,  6, 26, 37, 40, 33, 47,
    61, 45, 43, 21, 23, 58, 17, 10,
    51, 25, 36, 32, 60, 20, 57, 16,
    50, 31, 19, 15, 30, 14, 13, 12,
};

#define ls1(b) debruijn_table[(((b) & -(b)) * DEBRUIJN) >> 58]

// find index of least significant 1

/* Internal representation of the bit array. */
struct bitarray {
  /* The number of bits represented by this bit array. Need not be divisible by 8. */
  size_t bit_sz;
  /* The underlying memory buffer that stores the bits in packed form (8 per byte). */
  buf_t *buf;
};

bitarray_t *bitarray_new(size_t bit_sz) {
  assert(sizeof(buf_t) * 8 == BLOCK_SZ);
  /* Allocate an underlying buffer of ceil(bit_sz/8) bytes. */
  buf_t *buf = (buf_t *) calloc(1, (bit_sz + 7) / 8);
  if (buf == NULL)
    return NULL;
  bitarray_t *ret = malloc(sizeof(struct bitarray));
  if (ret == NULL) {
    free(buf);
    return NULL;
  }
  ret->buf = buf;
  ret->bit_sz = bit_sz;
  return ret;
}

void bitarray_free(bitarray_t *ba) {
  if (ba == NULL)
    return;
  free(ba->buf);
  ba->buf = NULL;
  free(ba);
}

size_t bitarray_get_bit_sz(bitarray_t *ba) {
  return ba->bit_sz;
}

/* Portable modulo operation that supports negative dividends. */
static size_t modulo(ssize_t n, size_t m) {
  /* See
  http://stackoverflow.com/questions/1907565/c-python-different-behaviour-of-the-modulo-operation */
  /* Mod may give different result if divisor is signed. */
  ssize_t sm = (ssize_t) m;
  assert(sm > 0);
  ssize_t ret = ((n % sm) + sm) % sm;
  assert(ret >= 0);
  return (size_t) ret;
}

static buf_t bitmask(size_t bit_index) {
  return ((buf_t) 1) << (bit_index % BLOCK_SZ);
}

bool bitarray_get(bitarray_t *ba, size_t bit_index) {
  assert(bit_index < ba->bit_sz);
  return (ba->buf[bit_index / BLOCK_SZ] & bitmask(bit_index)) ? true : false;
}

void bitarray_set(bitarray_t *ba, size_t bit_index, bool val) {
  assert(bit_index < ba->bit_sz);
  ba->buf[bit_index / BLOCK_SZ]
      = (ba->buf[bit_index / BLOCK_SZ] & ~bitmask(bit_index)) | (val ? bitmask(bit_index) : 0); 
}

size_t bitarray_count_block_flips(buf_t x, bool lastmsb) {
    x = x ^ ((x << 1) + lastmsb);
    // popcount
    x -= (x >> 1) & B0;
    x = (x & B1) + ((x >> 2) & B1);
    x = (x + (x >> 4)) & B2;
    x += x >> 8;
    x += x >> 16;
    x += x >> 32;
    return x & 0x7f;
}

size_t bitarray_count_flips(bitarray_t *ba, size_t bit_off, size_t bit_len) {
  assert(bit_off + bit_len <= ba->bit_sz);
  size_t block_begin = bit_off / BLOCK_SZ;
  size_t block_end = (bit_off + bit_len) / BLOCK_SZ;

  buf_t truncated;
  if (block_begin == block_end) {
      // rare special case
      size_t left_off = (BLOCK_SZ - bit_off - bit_len) % BLOCK_SZ;
      //printf("BSZ: %ld, BOFF: %ld, BLEN: %ld, LEFTOFF: %ld\n", BLOCK_SZ, bit_off, bit_len, left_off);
      //printf("trunc before: %d\n", ba->buf[block_begin]);
      truncated = (ba->buf[block_end] << left_off) >> (left_off + bit_off % 8);
      //truncated = preserve_only(ba->buf[block_begin], BLOCK_SZ - left_off) >> (bit_off % 8);
      //truncated = (truncated << left_off) >> (left_off + bit_off % 8);
      //printf("trunc after: %d\n", truncated);
      return bitarray_count_block_flips(truncated, truncated % 2);
  }

  size_t flips = 0;

  // first block
  truncated = ba->buf[block_begin] >> (bit_off % BLOCK_SZ);
  // set lastbit equal to firstbit to not add to flips
  flips += bitarray_count_block_flips(truncated, truncated % 2);

  size_t bi;
  for (bi = block_begin + 1; bi < block_end; bi++) {
      flips += bitarray_count_block_flips(ba->buf[bi], ba->buf[bi-1] & MSB_MASK); 
  }

  size_t left_off;
  // last block
  if ((bit_off + bit_len) % BLOCK_SZ == 0)
      left_off = 0;
  else 
      left_off = (BLOCK_SZ - bit_off - bit_len) % BLOCK_SZ;
  truncated = (truncated << left_off) >> left_off; 
  flips += bitarray_count_block_flips(truncated, ba->buf[bi-1] & MSB_MASK);
  return flips;
}

void bitarray_reverse(bitarray_t *ba, size_t bit_begin, size_t bit_end) {
    assert(bit_begin <= bit_end && bit_end < ba->bit_sz);
    size_t i, j;
    size_t limit = (bit_begin + bit_end + 1) / 2;
    for (i = bit_begin; i < limit; i++) {
        j = bit_end - i + bit_begin;
        //swap
        bitarray_set(ba, i, bitarray_get(ba, i) ^ bitarray_get(ba, j));
        bitarray_set(ba, j, bitarray_get(ba, i) ^ bitarray_get(ba, j));
        bitarray_set(ba, i, bitarray_get(ba, i) ^ bitarray_get(ba, j));
    }
}

/* Rotate substring left by the specified number of bits. */
static void bitarray_rotate_left(bitarray_t *ba, size_t bit_off, size_t bit_len, size_t bit_amount)
{
    if (bit_amount == 0) return;
    bitarray_reverse(ba, bit_off, bit_off + bit_amount - 1);
    bitarray_reverse(ba, bit_off + bit_amount, bit_off + bit_len - 1);
    bitarray_reverse(ba, bit_off, bit_off + bit_len - 1);
}

void bitarray_rotate(bitarray_t *ba, size_t bit_off, size_t bit_len, ssize_t bit_right_amount) {
  assert(bit_off + bit_len <= ba->bit_sz);
  if (bit_len == 0)
    return;
  /* Convert a rotate left or right to a left rotate only, and eliminate multiple full rotations. */
  bitarray_rotate_left(ba, bit_off, bit_len, modulo(-bit_right_amount, bit_len));
}
