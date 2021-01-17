cmake_minimum_required(VERSION 3.16)
project(FindSongDuplicates)

set(CMAKE_CXX_STANDARD 17)

add_executable(FindSongDuplicates main.cpp alg/WordDiffLite.cpp alg/Cell.cpp alg/Point.cpp alg/Cell.h alg/Point.h alg/Operation.cpp alg/Operation.h alg/WordDiffLite.h alg/Difference.cpp alg/Difference.h)