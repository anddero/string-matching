//
// Created by karlandero on 21.01.21.
//

#include <vector>
#include <algorithm>
#include "SearchQueryDifference.h"
#include "../util/StringUtil.h"
#include "DamerauLevenshtein.h"
#include "HungarianMatching.h"

const std::string WHITESPACE = " \t\r\n";
const std::string IGNORED_SYMBOLS = "\"#&'()*+,-./:;<=>?^_`~";
const std::string SPLIT_SYMBOLS = WHITESPACE + IGNORED_SYMBOLS;

std::vector<std::string> get_words(const std::string &phrase) {
    std::vector<std::string> words = Util::StringUtil::split(phrase, SPLIT_SYMBOLS);
    for (unsigned i = 0; i != words.size(); ++i) {
        if (words[i].empty()) {
            words.erase(words.begin() + i--);
        } else {
            words[i] = Util::StringUtil::lowercase(words[i]);
        }
    }
    return words;
}

std::vector<int> fix_indices_matching_smallest_by_largest(const std::vector<unsigned>& matching_smallest_by_largest_displaced, unsigned words_added_to_smallest_count) {
    std::vector<int> result(matching_smallest_by_largest_displaced.size());
    for (unsigned largest_index = 0; largest_index != matching_smallest_by_largest_displaced.size(); ++largest_index) {
        const unsigned smallest_index = matching_smallest_by_largest_displaced[largest_index];
        if (smallest_index >= words_added_to_smallest_count) {
            result[largest_index] = smallest_index - words_added_to_smallest_count;
        } else {
            result[largest_index] = -1; // Actually references an empty word which was inserted as padding
        }
    }
    return result;
}

std::vector<int> flip_matching_indices_shrinking(const std::vector<int> &matching_indices, const unsigned new_size) {
    std::vector<int> result(new_size);
    for (unsigned i = 0; i != matching_indices.size(); ++i) {
        if (matching_indices[i] >= 0) {
            result[matching_indices[i]] = i;
        }
    }
    return result;
}

std::vector<int> flip_matching_indices_expanding(const std::vector<int> &matching_indices, const unsigned new_size) {
    std::vector<int> result(new_size, -1);
    for (unsigned i = 0; i != matching_indices.size(); ++i) {
        result[matching_indices[i]] = i;
    }
    return result;
}

std::vector<int> flip_matching_indices(const std::vector<int> &matching_indices) {
    std::vector<int> result(matching_indices.size());
    for (unsigned i = 0; i != matching_indices.size(); ++i) {
        result[matching_indices[i]] = i;
    }
    return result;
}

SearchQueryDifferenceResult search_query_difference(const std::string &query1, const std::string &query2) {
    auto words1 = get_words(query1);
    auto words2 = get_words(query2);

    unsigned long long int words_count_1 = words1.size();
    unsigned long long int words_count_2 = words2.size();

    bool words_count_1_bigger = words_count_1 > words_count_2;
    
    auto& largest = words_count_1_bigger ? words1 : words2;
    auto& smallest = words_count_1_bigger ? words2 : words1;

    unsigned long long int matrix_dim = largest.size();

    if (matrix_dim == 0) return SearchQueryDifferenceResult(0, std::vector<int>());

    unsigned long long int inserted_empty_word_count = matrix_dim - smallest.size();
    if (matrix_dim > smallest.size()) {
        smallest.insert(smallest.begin(), inserted_empty_word_count, "");
    }

    unsigned matrix[matrix_dim * matrix_dim];
    for (unsigned row = 0; row != matrix_dim; ++row) {
        for (unsigned col = 0; col != matrix_dim; ++col) {
            matrix[row * matrix_dim + col] = damerau_levenshtein_simplified(largest[row], smallest[col]);
            // Here we are mapping each word in 'largest' to a row and each word in 'smallest' to a column.
            // So if we had a 3x3 matrix, then the difference value at [row=1,col=2] would be at matrix[1*3+2] and
            // represent the difference of the words largest[1] and smallest[2].
        }
    }

    const HungarianMatching::Result matching_result = HungarianMatching(matrix, (unsigned) matrix_dim).calculate();
    const auto& matching_smallest_by_largest = fix_indices_matching_smallest_by_largest(matching_result.matching_cols_by_row,inserted_empty_word_count);
    // In the result, we want to give a list of query2 indices where the actual index of the element is query1 index.
    // Everything is already correct, if query1 has more words (largest = query1, smallest = query2, 
    // so matching_smallest_by_largest is matching_query2_by_query1, which is what we want).
    // Otherwise, if largest = query2, we must flip the index list, to get matching_query2_by_query1.
    const auto& matching_query2_by_query1 = words_count_1_bigger ? matching_smallest_by_largest : flip_matching_indices_shrinking(
            matching_smallest_by_largest, matrix_dim - inserted_empty_word_count);
    return SearchQueryDifferenceResult(matching_result.difference, matching_query2_by_query1);
}

SearchQuerySimilarityResult search_query_similarity(const std::string &query1, const std::string &query2) {
    std::string filtered1 = Util::StringUtil::filter(query1, SPLIT_SYMBOLS);
    std::string filtered2 = Util::StringUtil::filter(query2, SPLIT_SYMBOLS);
    unsigned max_diff = (unsigned) std::max(filtered1.size(), filtered2.size());
    if (max_diff == 0) return SearchQuerySimilarityResult(1.0, std::vector<int>());
    const SearchQueryDifferenceResult diff_result = search_query_difference(query1, query2);
    unsigned diff_score = diff_result.difference;
    if (diff_score == 0) return SearchQuerySimilarityResult(1.0, diff_result.matching_query2_word_index_by_query1_word_index);
    if (diff_score > max_diff) return SearchQuerySimilarityResult(0.0, diff_result.matching_query2_word_index_by_query1_word_index);
    return SearchQuerySimilarityResult(1.f - (diff_score / (float) max_diff), diff_result.matching_query2_word_index_by_query1_word_index);
}
