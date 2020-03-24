#include <emmintrin.h>

/* Uses SIMD instructions to quickly transpose a 4x4 matrix. */
inline void
transpose_4x4( const pixel_t src0[4], const pixel_t src1[4], 
               const pixel_t src2[4], const pixel_t src3[4], 
               pixel_t dest0[4], pixel_t dest1[4], 
               pixel_t dest2[4], pixel_t dest3[4] ) 
{
  __m128 tmp3, tmp2, tmp1, tmp0;                          
  __m128 row3, row2, row1, row0;                          

  /* Load 4x4 matrix from memory into four SEE registers. */
  row0 = _mm_load_ps( (float*)src0 );
  row1 = _mm_load_ps( (float*)src1 );
  row2 = _mm_load_ps( (float*)src2 );
  row3 = _mm_load_ps( (float*)src3 );

  /* Interleave bottom/top two pixels from two SSE registers with each other 
   * into a single SSE register. */
  tmp0 = _mm_unpacklo_ps( row0, row1 );               
  tmp2 = _mm_unpacklo_ps( row2, row3 );               
  tmp1 = _mm_unpackhi_ps( row0, row1 );               
  tmp3 = _mm_unpackhi_ps( row2, row3 );               
                                                          
  /* Move bottom/top two pixels from two SSE registers into one SSE register. */
  row0 = _mm_movelh_ps( tmp0, tmp2 );                     
  row1 = _mm_movehl_ps( tmp2, tmp0 );                     
  row2 = _mm_movelh_ps( tmp1, tmp3 );                     
  row3 = _mm_movehl_ps( tmp3, tmp1 );                     

  /* Store 4x4 matrix from all four SSE registers into memory. */
  _mm_store_ps( (float*)dest0, row0 );
  _mm_store_ps( (float*)dest1, row1 );
  _mm_store_ps( (float*)dest2, row2 );
  _mm_store_ps( (float*)dest3, row3 );
}
