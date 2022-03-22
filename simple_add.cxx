#include "tools.h"

#include <iostream>
#include <functional>
#include <memory.h>

#include <tmmintrin.h>
int add_SSE(int size, float *first_array, float *second_array) {
  int i = 0;
  for (; i + 4 <= size; i += 4) {

	__m128 packed = _mm_set_ps(first_array[i], first_array[i + 1], first_array[i + 2], first_array[i + 3]);
	__m128 second_packed = _mm_set_ps(second_array[i], second_array[i + 1], second_array[i + 2], second_array[i + 3]);
	__m128 result = _mm_add_ps(packed, second_packed);

	first_array[i] = result[3];
	first_array[i + 1] = result[2];
	first_array[i + 2] = result[1];
	first_array[i + 3] = result[0];

  }
  for (; i < size; ++i)
	first_array[i] += second_array[i];

  return 0;
}

int add(int size, float *first_array, const float *second_array) {
  for (size_t i = 0; i < size; i++)
	first_array[i] += second_array[i];
  return 0;
}

int main() {

  const int size = 50'000;
  float first_array[size];
  float second_array[size];

  CleanUp(size, first_array, second_array);

  std::function<void(void)> using_simd = [size, &first_array, &second_array]() {
	add_SSE(size, first_array, second_array);
  };

  std::function<void(void)> using_simple_loop = [size, &first_array, &second_array]() {
	add(size, first_array, second_array);
  };

  auto simd_times = measure_time(using_simd, 1000);

  float copy[size];
  for (int i = 0; i < size; i++)
	copy[i] = first_array[i];

  CleanUp(size, first_array, second_array);

  auto loop_times = measure_time(using_simple_loop, 1000);

  Check(size, first_array, copy);

  std::cout << "Average time Using SSE:\t" << Avg(simd_times) << " microseconds\n";
  std::cout << "Average time Using Simple Loop:\t" << Avg(loop_times) << " microseconds\n";
  return 0;
}