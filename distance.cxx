#include <iostream>
#include <memory.h>
#include <cmath>
#include "tools.h"


#include <emmintrin.h>
int dist_SSE(int size, float *first_array, float *second_array) {
  int i = 0;
  for (; i + 4 <= size; i += 4) {
	__m128 first_values = _mm_loadu_ps(&first_array[i]);
	__m128 second_values = _mm_loadu_ps(&first_array[i]);

	__m128 difference = _mm_sub_ps(first_values, second_values);
	__m128 power = _mm_mul_ps(difference, difference);
	__m128 sqrt = _mm_sqrt_ps(power);

	_mm_storeu_ps(&first_array[i], sqrt);
  }
  for (; i < size; ++i) {
	first_array[i] = sqrt(pow(first_array[i] - second_array[i], 2));
  }
  return 0;
}

int dist_simple(int size, float *first_array, float *second_array) {
  for (size_t i = 0; i < size; i++) {
	first_array[i] = sqrt(pow(first_array[i] - second_array[i], 2));
  }
  return 0;
}

int main() {
  const int size = 50'000;
  float first_array[size];
  float second_array[size];

  CleanUp(size, first_array, second_array);

  std::function<void(void)> using_AWX = [size, &first_array, &second_array]() {
	dist_SSE(size, first_array, second_array);
  };

  std::function<void(void)> using_simple_loop = [size, &first_array, &second_array]() {
	dist_simple(size, first_array, second_array);
  };

  auto awx_times = measure_time(using_AWX, 1000);

  float copy[size];
  for (int i = 0; i < size; i++)
	copy[i] = first_array[i];

  CleanUp(size, first_array, second_array);

  auto loop_times = measure_time(using_simple_loop, 1000);

  Check(size, first_array, copy);

  std::cout << "Average time Using SSE:\t" << Avg(awx_times) << " microseconds\n";
  std::cout << "Average time Using Simple Loop:\t" << Avg(loop_times) << " microseconds\n";
  return 0;
}

