//
// Created by andde on 11-Jan-21.
//

#include "Difference.h"

Difference::Difference(const unsigned long long word_one_length,
                       const unsigned long long word_two_length,
                       const unsigned long long int match_count,
                       const long long int expansion_size)
        : match_count{match_count},
          change_count{(word_one_length + word_two_length + expansion_size) / 2ull - match_count},
          expansion_size{expansion_size},
          ratio{change_count / (float) std::max(std::max(word_one_length, word_two_length), 1ull)} {}

bool Difference::operator==(const Difference &rhs) const {
    return match_count == rhs.match_count &&
           change_count == rhs.change_count &&
           expansion_size == rhs.expansion_size &&
           ratio == rhs.ratio &&
           operations == rhs.operations;
}

bool Difference::operator!=(const Difference &rhs) const {
    return !(rhs == *this);
}

std::ostream &operator<<(std::ostream &os, const Difference &difference) {
    os << "match_count: " << difference.match_count << " change_count: " << difference.change_count
       << " expansion_size: " << difference.expansion_size << " ratio: " << difference.ratio << " operations: "
       << difference.operations.size();
    return os;
}
