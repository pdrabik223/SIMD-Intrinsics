//
// Created by piotr on 21/03/2022.
//

#include "tools.h"

std::vector<size_t> measure_time(std::function<void(void)> function, size_t samples = 1000) {
  std::vector<size_t> results;
  results.reserve(samples);

  for (size_t i = 0; i < samples; i++) {
	auto start = high_resolution_clock::now();
	function();
	auto end = high_resolution_clock::now();
	results.push_back(std::chrono::duration_cast<microseconds>(end - start).count());
  }
  return results;
}
void CleanUp(const int size, float *first_array, float *second_array) {
  for (int i = 0; i < size; i++) {
	first_array[i] = 0;
	second_array[i] = i % 10;
  }
}
void Check(int size, float *first_array, float *copy) {
  for (int i = 0; i < size; i++) {
	if (std::abs(copy[i] - first_array[i]) > 0.1) {
	  std::clog << "Values are not the same: " << std::abs(copy[i] - first_array[i]) << "\t";
	  exit(1);
	}
  }
}
