//
// Created by karlandero on 20.01.21.
//

#include "Random.h"

Util::Random::Random() : rd{new std::random_device()},
                         mt{new std::mt19937((*rd)())},
                         dist{new std::uniform_real_distribution<long double>(0.0l, 1.0l)} {
}

Util::Random::~Random() {
    delete dist;
    delete mt;
    delete rd;
}

template<typename NumericType>
NumericType Util::Random::rand(NumericType a, NumericType b) {
    NumericType res = a + (NumericType) ((*dist)(*mt) * (b - a));
    if (res < a) return a;
    if (res > b) return b;
    return res;
}

bool Util::Random::rand_bool() {
    return (*dist)(*mt) < 0.5;
}
