#ifndef __clang__
static_assert(false, "Compile using Clang");
#endif

#include <iostream>
#include <vector>
#include <immintrin.h>
#include <tmmintrin.h>
#include <smmintrin.h>
#include <chrono>

#define ARRAY_SIZE (1024*1024*300)

using std::chrono::high_resolution_clock;
using std::chrono::microseconds;
#define TIME_SCOPE(scope, name)   do { \
auto start = high_resolution_clock::now(); \
scope \
auto end = high_resolution_clock::now(); \
std::cout << name << " took:\t" << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();\
} while (false)

std::vector<int32_t> array(ARRAY_SIZE, 0);


int main() {
  srand(time(NULL));
  TIME_SCOPE({
               for (size_t i = 0; i < ARRAY_SIZE / 4; i += 4) {
                 __m128i packed = _mm_set_epi32(array[i], array[i + 1], array[i + 2], array[i + 3]);
                 __m128i indexes = _mm_set_epi32(i, i + 1, i + 2, i + 3);
                 __m128i result = _mm_add_epi32(packed, indexes);
                 array[i] = _mm_extract_epi32(result, 3);
                 array[i + 1] = _mm_extract_epi32(result, 2);
                 array[i + 2] = _mm_extract_epi32(result, 1);
                 array[i + 3] = _mm_extract_epi32(result, 0);
               }
             }, "Using SIMD");
  std::cout << std::endl;

  int index = rand() % INT32_MAX;
  std::cout << index << "\t" << array[index] << std::endl;

  std::fill(array.begin(), array.end(), 0);
  TIME_SCOPE({
               for (size_t i = 0; i < ARRAY_SIZE; i++) {
                 array[i] += i;
               }
             }, "Simple loop");

  std::cout << std::endl;
  std::cout << index << "\t" << array[index] << std::endl;

  return 0;
}
