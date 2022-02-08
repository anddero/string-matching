//
// Created by karlandero on 18.01.21.
//

#include <stdexcept>
#include <limits>
#include "DamerauLevenshtein.h"

const auto MAX_UNSIGNED = std::numeric_limits<unsigned>::max();

unsigned damerau_levenshtein_simplified(const std::string &word1, const std::string &word2) {
    const std::string& longest_word = word1.size() > word2.size() ? word1 : word2;
    const std::string& shortest_word = word1.size() > word2.size() ? word2 : word1;

    if (longest_word.size() > MAX_UNSIGNED) {
        throw std::runtime_error("Observable word length exceeds supported maximum length");
    }

    const auto longest_word_length = (unsigned) longest_word.size();
    const auto shortest_word_length = (unsigned) shortest_word.size();

    unsigned first_nonmatching = 0;
    for (unsigned i = 0; i != shortest_word_length; ++i) {
        if (longest_word[i] != shortest_word[i]) {
            break;
        }
        ++first_nonmatching;
    }
    unsigned first_rev_nonmatching = 0;
    for (unsigned i = 0; i != shortest_word_length; ++i) {
        if (longest_word[longest_word_length - i - 1] != shortest_word[shortest_word_length - i - 1]) {
            break;
        }
        ++first_rev_nonmatching;
    }

    const unsigned shortest_start = first_nonmatching;
    const unsigned longest_start = first_nonmatching;
    const unsigned shortest_end = shortest_word_length - first_rev_nonmatching;
    const unsigned longest_end = longest_word_length - first_rev_nonmatching;

    if (shortest_end < shortest_start) {
        return longest_word_length - shortest_word_length;
    }

    const unsigned longest_observable_length = longest_end - longest_start + 1;
    const unsigned shortest_observable_length = shortest_end - shortest_start + 1;


    if (longest_observable_length * (unsigned long long) shortest_observable_length > 1000000000ull) {
        throw std::runtime_error("Observable word length exceeds supported maximum length");
    }

    unsigned t2 = 0;
    unsigned t1 = 0;
    unsigned sl[shortest_observable_length];
    unsigned l[shortest_observable_length];

    unsigned swap_score;
    unsigned repl_score;
    unsigned del_score;
    unsigned add_score;

    for (unsigned i = 0; i != shortest_observable_length; ++i) {
        l[i] = i;
    }

    for (unsigned j = 1; j != longest_observable_length; ++j) {
        for (unsigned i = 0; i != shortest_observable_length; ++i) {
            swap_score = MAX_UNSIGNED;
            if (
                    i > 1
                    && j > 1
                    && longest_word[longest_start + j - 2] == shortest_word[shortest_start + i - 1]
                    && longest_word[longest_start + j - 1] == shortest_word[shortest_start + i - 2]
            ) {
                swap_score = t2 + 1;
            }
            repl_score = MAX_UNSIGNED;
            del_score = MAX_UNSIGNED;
            if (i > 0) {
                repl_score = sl[i - 1] + (longest_word[longest_start + j - 1] != shortest_word[shortest_start + i - 1]);
                del_score = l[i - 1] + 1;
            }
            add_score = l[i] + 1;

            t2 = t1;
            t1 = sl[i];
            sl[i] = l[i];
            l[i] = std::min(
                    std::min(swap_score, repl_score),
                    std::min(del_score, add_score)
            );
        }
    }

    return l[shortest_observable_length - 1];
}
