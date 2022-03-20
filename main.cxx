#ifndef __clang__
static_assert(false && "Compile using Clang");
#endif

#include <iostream>
#include <immintrin.h>
#include <tmmintrin.h>
#include <smmintrin.h>

int main() {

  __m128i a = _mm_set_epi32(-1, -2, -3, -4);

  a = _mm_abs_epi32(a);

  std::cout << 1 << "\t" << _mm_extract_epi32(a, 0) << std::endl;
  std::cout << 2 << "\t" << _mm_extract_epi32(a, 1) << std::endl;
  std::cout << 3 << "\t" << _mm_extract_epi32(a, 2) << std::endl;
  std::cout << 4 << "\t" << _mm_extract_epi32(a, 3) << std::endl;

  return 0;
}
