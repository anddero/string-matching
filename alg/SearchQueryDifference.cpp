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

std::string normalize_phrase(const std::string &phrase) {
    auto words = get_words(phrase);
    std::sort(words.begin(), words.end());
    return Util::StringUtil::join(words, " ");
}

HungarianMatching::Result search_query_difference(const std::string &query1, const std::string &query2) {
    auto words1 = get_words(query1);
    auto words2 = get_words(query2);

    auto& largest = words1.size() > words2.size() ? words1 : words2;
    auto& smallest = words1.size() > words2.size() ? words2 : words1;

    if (largest.empty()) return HungarianMatching::Result(0, std::vector<unsigned>());

    if (largest.size() > smallest.size()) {
        smallest.insert(smallest.begin(), largest.size() - smallest.size(), "");
    }

    unsigned matrix[largest.size() * largest.size()];
    for (unsigned row = 0; row != largest.size(); ++row) {
        for (unsigned col = 0; col != largest.size(); ++col) {
            matrix[row * largest.size() + col] = damerau_levenshtein_simplified(largest[row], smallest[col]);
        }
    }

    return HungarianMatching(matrix, (unsigned) largest.size()).calculate();
}

SearchQuerySimilarityResult search_query_similarity(const std::string &query1, const std::string &query2) {
    std::string filtered1 = Util::StringUtil::filter(query1, SPLIT_SYMBOLS);
    std::string filtered2 = Util::StringUtil::filter(query2, SPLIT_SYMBOLS);
    unsigned max_diff = (unsigned) std::max(filtered1.size(), filtered2.size());
    if (max_diff == 0) return SearchQuerySimilarityResult(1.0, std::vector<unsigned>());
    const HungarianMatching::Result diff_result = search_query_difference(query1, query2);
    unsigned diff_score = diff_result.difference;
    if (diff_score == 0) return SearchQuerySimilarityResult(1.0, diff_result.matching_cols_by_row);
    if (diff_score > max_diff) return SearchQuerySimilarityResult(0.0, diff_result.matching_cols_by_row);
    return SearchQuerySimilarityResult(1.f - (diff_score / (float) max_diff), diff_result.matching_cols_by_row);
}
