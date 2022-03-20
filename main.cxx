//#ifndef __clang__
//static_assert(false && "Compile using Clang");
//#endif

#include <iostream>
#include <immintrin.h>
#include <tmmintrin.h>
#include <smmintrin.h>
#include <vector>

template<class T>
double Avg(const std::vector<T> &vector) {
  double sum = 0;
  for (auto i: vector) { sum += i; }
  return sum / vector.size();
}

int main() {


  return 0;
}
