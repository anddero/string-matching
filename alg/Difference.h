//
// Created by andde on 11-Jan-21.
//

#ifndef FINDSONGDUPLICATES_DIFFERENCE_H
#define FINDSONGDUPLICATES_DIFFERENCE_H


#include <vector>
#include <ostream>
#include "Operation.h"

struct Difference { // Checked, Test all methods
    /**
     * Number of matching characters.
     */
    unsigned long long match_count;
    /**
     * Number of changed characters (replacements + inserts + deletions).
     */
    unsigned long long change_count;
    /**
     * Number of stand-alone single character operations (inserts + deletions, where no deletion and insertion is from
     * the same group). In other words, number of changed characters which cannot be part of a replacement.
     */
    long long expansion_size;
    /**
     * Ratio of change count to the longest word length: change_count / max(len1, len2).
     * The smaller the ratio, the more similar the words are, with 0 being equal.
     */
    float ratio;
    /**
     * The changes which must be made to word1 in order to achieve word2.
     */
    std::vector<Operation> operations;

    Difference(unsigned long long word_one_length, unsigned long long word_two_length,
               unsigned long long int match_count, long long int expansion_size);

    bool operator==(const Difference &rhs) const;

    bool operator!=(const Difference &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const Difference &difference);
};


#endif //FINDSONGDUPLICATES_DIFFERENCE_H
