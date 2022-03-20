#ifndef __clang__
static_assert(false, "Compile using Clang");
#endif

#include <iostream>
#include <vector>
#include <immintrin.h>
#include <tmmintrin.h>
#include <smmintrin.h>
#include <chrono>
#include <functional>

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
std::vector<int32_t> array2(ARRAY_SIZE, 0);

std::vector<microseconds> measure_time(std::function<void(void)> function, size_t samples = 1000) {
  std::vector<microseconds> results;
  results.reserve(samples);

  for (size_t i = 0; i < samples; i++) {
//    std::fill(array.begin(), array.end(), 0);
//    std::fill(array.begin(), array.end(), 0);
    auto start = high_resolution_clock::now();
    function();
    auto end = high_resolution_clock::now();
    results.push_back(std::chrono::duration_cast<microseconds>(end - start));
  }
  return results;
}

int main() {
  srand(time(NULL));
  std::function<void(void)> using_simd = []() {
    for (size_t i = 0; i < ARRAY_SIZE / 4; i += 4) {
      __m128i packed = _mm_set_epi32(array[i], array[i + 1], array[i + 2], array[i + 3]);
      __m128i indexes = _mm_set_epi32(i, i + 1, i + 2, i + 3);
      __m128i result = _mm_add_epi32(packed, indexes);
      array[i] = _mm_extract_epi32(result, 3);
      array[i + 1] = _mm_extract_epi32(result, 2);
      array[i + 2] = _mm_extract_epi32(result, 1);
      array[i + 3] = _mm_extract_epi32(result, 0);
    }
  };
  std::function<void(void)> using_simple_loop = []() {
    for (size_t i = 0; i < ARRAY_SIZE; i++) {
      array2[i] += i;
    }
  };

  auto simd_times = measure_time(using_simd);
  auto loop_times = measure_time(using_simple_loop);

  std::cout << "Validation ... ";
  for (int i = 0; i < ARRAY_SIZE; i++) {
    if (array[i] != array2[i]) {
      throw std::runtime_error("Values in both arrays are not the same");
    }
  }
  std::cout << "ok";
  return 0;
}
