//
// Created by andde on 17-Jan-21.
//

#include <stdexcept>
#include "Levenshtein.h"

unsigned levenshtein(const std::string& word1, const std::string& word2) {
    const std::string& longest_word = word1.size() > word2.size() ? word1 : word2;
    const std::string& shortest_word = word1.size() > word2.size() ? word2 : word1;

    if (longest_word.size() > std::numeric_limits<unsigned>::max()) {
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

    unsigned shortest_start = first_nonmatching;
    unsigned longest_start = first_nonmatching;
    unsigned shortest_end = shortest_word_length - first_rev_nonmatching;
    unsigned longest_end = longest_word_length - first_rev_nonmatching;

    if (shortest_end < shortest_start) {
        return longest_word_length - shortest_word_length;
    }

    unsigned long long longest_observable_length = longest_end - longest_start + 1;
    unsigned long long shortest_observable_length = shortest_end - shortest_start + 1;

    unsigned M[shortest_observable_length];
    unsigned d = 0;
    unsigned s = 0;

    for (unsigned i = 0; i != shortest_observable_length; ++i) {
        M[i] = i;
    }

    for (unsigned j = 1; j != longest_observable_length; ++j) {
        for (unsigned i = 0; i != shortest_observable_length; ++i) {
            s = d;
            d = M[i];
            if (i == 0) {
                M[i] = j;
            } else {
                M[i] = std::min(
                        std::min(
                                M[i] + 1,
                                M[i - 1] + 1
                        ),
                        s + (longest_word[longest_start + j - 1] != shortest_word[shortest_start + i - 1])
                );
            }
        }
    }

    return M[shortest_observable_length - 1];
}
