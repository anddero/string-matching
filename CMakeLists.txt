cmake_minimum_required(VERSION 3.10)
project(FindSongDuplicates)

set(CMAKE_CXX_STANDARD 17)

add_executable(FindSongDuplicates main.cpp
        alg/Levenshtein.cpp alg/Levenshtein.h
        alg/DamerauLevenshtein.cpp alg/DamerauLevenshtein.h
        util/LoadingBar.cpp util/LoadingBar.h
        util/Random.tpp util/Random.h
        util/TestSuite.tpp util/TestSuite.h
        util/StringUtil.cpp util/StringUtil.h
        alg/SearchQueryDifference.cpp alg/SearchQueryDifference.h
        alg/DuplicateFilter.cpp alg/DuplicateFilter.h
        alg/HungarianMatching.cpp alg/HungarianMatching.h)
