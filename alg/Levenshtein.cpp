//
// Created by andde on 17-Jan-21.
//

#include <stdexcept>
#include "Levenshtein.h"

unsigned long long levenshtein(const std::string& word1, const std::string& word2) {
    const std::string& longest_word = word1.size() > word2.size() ? word1 : word2;
    const std::string& shortest_word = word1.size() > word2.size() ? word2 : word1;

    unsigned long long first_nonmatching = 0;
    for (auto i = 0ull; i != shortest_word.length(); ++i) {
        if (longest_word[i] != shortest_word[i]) {
            break;
        }
        ++first_nonmatching;
    }
    unsigned long long first_rev_nonmatching = 0;
    for (auto i = 0ull; i != shortest_word.length(); ++i) {
        if (longest_word[longest_word.size() - i - 1] != shortest_word[shortest_word.size() - i - 1]) {
            break;
        }
        ++first_rev_nonmatching;
    }

    unsigned long long shortest_start = first_nonmatching;
    unsigned long long longest_start = first_nonmatching;
    unsigned long long shortest_end = shortest_word.length() - first_rev_nonmatching;
    unsigned long long longest_end = longest_word.length() - first_rev_nonmatching;

    if (shortest_end < shortest_start) {
        return longest_word.size() - shortest_word.size();
    }

    unsigned long long longest_length = longest_end - longest_start + 1;
    unsigned long long shortest_length = shortest_end - shortest_start + 1;


    if (longest_length > std::numeric_limits<unsigned>::max()) {
        throw std::runtime_error("Observable word length exceeds supported maximum length");
    }

    unsigned M[shortest_length];
    unsigned d, s = 0;

    for (unsigned i = 0; i != shortest_length; ++i) {
        M[i] = i;
    }

    for (unsigned j = 1; j != longest_length; ++j) {
        for (unsigned i = 0; i != shortest_length; ++i) {
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

    return M[shortest_length - 1];
}
