#include <iostream>
#include <vector>
#include <immintrin.h>
#include <tmmintrin.h>
#include <smmintrin.h>
#include <smmintrin.h>
#include <immintrin.h>
#include <immintrin.h>
#include <emmintrin.h>
#include <vector>
#include <chrono>
#include <functional>
#include <cassert>

#include <array>
#include <cmath>
using std::chrono::high_resolution_clock;
using std::chrono::microseconds;
// 1. czy chcemy mieć N wymiarowe macierze
// 2. czy chcemy mnożenie macierzy
// Gaussian blur

//template <size_t W,size_t H>
//struct Mat{
//  std::array<,int> data;
//
//};
//
//void dotFourByFourMatrix(const Mat4 *left, const Mat4 *right, Mat4 *result) {
//  const __m128 BCx = _mm_load_ps((float *)&B.Row0);
//  const __m128 BCy = _mm_load_ps((float *)&B.Row1);
//  const __m128 BCz = _mm_load_ps((float *)&B.Row2);
//  const __m128 BCw = _mm_load_ps((float *)&B.Row3);
//  float *leftRowPointer = &left->Row0;
//  float *resultRowPointer = &result->Row0;
//  for (unsigned int i = 0; i < 4; ++i, leftRowPointer += 4, resultRowPointer += 4) {
//	__m128 ARx = _mm_set1_ps(leftRowPointer[0]);
//	__m128 ARy = _mm_set1_ps(leftRowPointer[1]);
//	__m128 ARz = _mm_set1_ps(leftRowPointer[2]);
//	__m128 ARw = _mm_set1_ps(leftRowPointer[3]);
//	__m128 X = ARx * BCx;
//	__m128 Y = ARy * BCy;
//	__m128 Z = ARz * BCz;
//	__m128 W = ARw * BCw;
//	__m128 R = X + Y + Z + W;
//	_mm_store_ps(resultRowPointer, R);
//  }
//}
//void dotDourByFourMatrixCasual(const Mat4 *left,const Mat4 *right,const Mat4 *result){
//  for (int i = 0; i < 4; i++)
//	for (int j = 0; j < 4; j++) {
//	  for (int k = 0; k < 4; k++)
//		result[] += matrix_a.Get(i, k) * matrix_b.Get(k, j);
//}

template<class T>
double Avg(const std::vector<T> &vector) {
  double sum = 0;
  for (auto i: vector) { sum += i; }
  return sum / vector.size();
}

int add_AVX(int size, float *first_array, float *second_array) {
  int i = 0;
  for (; i + 4 <= size; i += 4) {
	__m128 first_values = _mm_load_ps1(&first_array[i]);
	__m128 second_values = _mm_load_ps1(&second_array[i]);

	__m128 difference = _mm_sub_ps(first_values, second_values);
	__m128 power = _mm_mul_ps(difference, difference);
	__m128 sqrt = _mm_sqrt_ss(power);

	_mm_storer_ps(&first_array[i], sqrt);
  }
  // handle left-over
  for (; i < size; ++i) {
	first_array[i] += second_array[i];
  }
  return 8;
}

std::vector<size_t> measure_time(std::function<void(void)> function, size_t samples = 1000) {
  std::vector<size_t> results;
  results.reserve(samples);

  for (size_t i = 0; i < samples; i++) {
//    std::fill(array.begin(), array.end(), 0);
//    std::fill(array2.begin(), array2.end(), 0);
	auto start = high_resolution_clock::now();
	function();
	auto end = high_resolution_clock::now();
	results.push_back(std::chrono::duration_cast<microseconds>(end - start).count());
  }
  return results;
}

int main() {
  srand(time(NULL));
  const int size = 20000;
  float first_array[size];
  float second_array[size];

  for (int i = 0; i < size; i++) {
	first_array[i] = 0;
	second_array[i] = i%10;
  }
  std::function<void(void)> using_AWX = [size, &first_array, &second_array]() {
	add_AVX(size, first_array, second_array);
  };

  std::function<void(void)> using_simple_loop = [size, &first_array, &second_array]() {
	for (size_t i = 0; i < size; i++) {
	  first_array[i] = sqrt(pow(first_array[i] - second_array[i], 2));
	}
  };
  std::cout << "SIMD...\t";
  auto awx_times = measure_time(using_AWX, 1000);

  std::cout << "done\nValidation ... ";
  for (int i = 0; i < 100; i++) {
	std::cout << first_array[rand() % size] - second_array[rand() % size];
  }

  for (int i = 0; i < size; i++) {
	first_array[i] = 0;
	second_array[i] = i%10;
  }

  std::cout << "done\nSimple loop...\t";
  auto loop_times = measure_time(using_simple_loop, 1000);

  std::cout << "done\nValidation ... ";
  for (int i = 0; i < 100; i++) {
	std::cout << first_array[rand() % size] - second_array[rand() % size];
  }
  std::cout << "ok\n";

  std::cout << "Average time Using AWX:\t" << Avg(awx_times) << " microseconds\n";
  std::cout << "Average time Using Simple Loop:\t" << Avg(loop_times) << " microseconds\n";
  return 0;
}