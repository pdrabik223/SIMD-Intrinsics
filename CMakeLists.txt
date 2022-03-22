cmake_minimum_required(VERSION 3.10)
project(SIMD)
set(TARGET SIMD)

set(CMAKE_CXX_STANDARD 17)
#add_compile_options(-march=native)
add_compile_options(-march=native -ffast-math)


add_executable(simple_add simple_add.cxx tools.h tools.cxx)

add_executable(fast_add fast_add.cxx tools.h tools.cxx)

add_executable(distance distance.cxx tools.h tools.cxx)

add_executable(hard_one hard.cxx tools.h tools.cxx)