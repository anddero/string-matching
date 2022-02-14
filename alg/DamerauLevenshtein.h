//
// Created by karlandero on 18.01.21.
//

#ifndef STRINGMATCHING_DAMERAULEVENSHTEIN_H
#define STRINGMATCHING_DAMERAULEVENSHTEIN_H

#include <string>

unsigned damerau_levenshtein_simplified(const std::string &, const std::string &);

// Damera-Levenshtein was not implemented due to significant increased memory use (n*m bytes) and asymptotic complexity
// in case of comparing words of Unicode character set (a hash-set would need to be seeked n*m times, where the
// hash-set's size would be the number of different characters in the shorter word). The gain would only be that an
// arbitrary number of characters may be either inserted or deleted in the middle of a swap operation of two consecutive
// characters. However, certain restrictions would apply to customizing the costs of the operations.

#endif //STRINGMATCHING_DAMERAULEVENSHTEIN_H
