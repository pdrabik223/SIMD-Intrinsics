//
// Created by piotr on 21/03/2022.
//

#ifndef SIMD__TOOLS_H_
#define SIMD__TOOLS_H_
#include <chrono>
#include <vector>
#include <functional>
#include <iostream>
template<class T>
double Avg(const std::vector<T> &vector) {
  double sum = 0;
  for (auto i: vector) { sum += i; }
  return sum / vector.size();
}

using std::chrono::high_resolution_clock;
using std::chrono::microseconds;

std::vector<size_t> measure_time(std::function<void(void)> function, size_t samples);

void CleanUp(int size, float *first_array, float *second_array);
void Check(int size, float *first_array, float *copy);
#endif //SIMD__TOOLS_H_
