#include <iostream>
#include <chrono>
#include <functional>
#include <cassert>
#include <memory.h>
#include <cmath>
#include "tools.h"



#include <immintrin.h>
int dist_AVX(int size, float *first_array, float *second_array) {
  int i = 0;
  for (; i + 8 <= size; i += 8) {
	__m256 first_values = _mm256_loadu2_m128(&first_array[i], &first_array[i + 4]);
	__m256 second_values = _mm256_loadu2_m128(&first_array[i], &first_array[i + 4]);

	__m256 difference = _mm256_sub_ps(first_values, second_values);
	__m256 power = _mm256_mul_ps(difference, difference);
	__m256 sqrt = _mm256_sqrt_ps(power);

	_mm256_storeu_ps(&first_array[i], sqrt);
  }
  for (; i < size; ++i) {
	first_array[i] = sqrt(pow(first_array[i] - second_array[i], 2));
  }
  return 0;
}
int dist(int size, float *first_array, float *second_array) {
  for (size_t i = 0; i < size; i++) {
	first_array[i] = sqrt(pow(first_array[i] - second_array[i], 2));
  }
  return 0;
}

int main() {
  srand(time(NULL));
  const int size = 50'000;
  float first_array[size];
  float second_array[size];

  CleanUp(size, first_array, second_array);

  std::function<void(void)> using_AWX = [size, &first_array, &second_array]() {
	dist_AVX(size, first_array, second_array);
  };

  std::function<void(void)> using_simple_loop = [size, &first_array, &second_array]() {
	dist(size, first_array, second_array);
  };

  auto awx_times = measure_time(using_AWX, 1000);

  float copy[size];
  for (int i = 0; i < size; i++)
	copy[i] = first_array[i];

  CleanUp(size, first_array, second_array);

  auto loop_times = measure_time(using_simple_loop, 1000);

  Check(size, first_array, copy);

  std::cout << "Average time Using AVX:\t" << Avg(awx_times) << " microseconds\n";
  std::cout << "Average time Using Simple Loop:\t" << Avg(loop_times) << " microseconds\n";
  return 0;
}



