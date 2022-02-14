//
// Created by karlandero on 20.01.21.
//

#ifndef STRINGMATCHING_RANDOM_H
#define STRINGMATCHING_RANDOM_H

#include <random>

namespace Util {

    class Random {

        std::random_device *rd;
        std::mt19937 *mt;
        std::uniform_real_distribution<long double> *dist;

    public:
        Random();

        ~Random();

        template<typename NumericType>
        NumericType rand(NumericType a, NumericType b); // TODO Test, also test negative ranges

        bool rand_bool(); // TODO Test
    };

}

#include "Random.tpp"

#endif //STRINGMATCHING_RANDOM_H
