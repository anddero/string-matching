//
// Created by karlandero on 21.01.21.
//

#ifndef FINDSONGDUPLICATES_SEARCHQUERYDIFFERENCE_H
#define FINDSONGDUPLICATES_SEARCHQUERYDIFFERENCE_H


#include <string>
#include "HungarianMatching.h"

struct SearchQuerySimilarityResult {
    float similarity;
    std::vector<int> matching_query2_word_index_by_query1_word_index;

    SearchQuerySimilarityResult(float similarity, const std::vector<int> &matching_query2_word_index_by_query1_word_index)
            : similarity(similarity)
            , matching_query2_word_index_by_query1_word_index(matching_query2_word_index_by_query1_word_index) {}
};

struct SearchQueryDifferenceResult {
    unsigned difference;
    std::vector<int> matching_query2_word_index_by_query1_word_index;

    SearchQueryDifferenceResult(unsigned int difference,
                                const std::vector<int> &matchingQuery2WordIndexByQuery1WordIndex)
        : difference(difference)
        , matching_query2_word_index_by_query1_word_index(matchingQuery2WordIndexByQuery1WordIndex) {}
};

std::string normalize_phrase(const std::string &phrase, const std::string &source_phrase_normalized = "");

SearchQueryDifferenceResult search_query_difference(const std::string &query1, const std::string &query2);

SearchQuerySimilarityResult search_query_similarity(const std::string &query1, const std::string &query2);


#endif //FINDSONGDUPLICATES_SEARCHQUERYDIFFERENCE_H
