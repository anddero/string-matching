//
// Created by karlandero on 21.01.21.
//

#ifndef FINDSONGDUPLICATES_SEARCHQUERYDIFFERENCE_H
#define FINDSONGDUPLICATES_SEARCHQUERYDIFFERENCE_H


#include <string>
#include "HungarianMatching.h"

struct SearchQuerySimilarityResult {
    float similarity;
    std::vector<unsigned> matching_source_word_indices;

    SearchQuerySimilarityResult(float similarity, const std::vector<unsigned int> &matchingSourceWordIndices)
            : similarity(similarity)
            , matching_source_word_indices(matchingSourceWordIndices) {}
};

std::string normalize_phrase(const std::string &phrase);

HungarianMatching::Result search_query_difference(const std::string &, const std::string &);

SearchQuerySimilarityResult search_query_similarity(const std::string &, const std::string &);


#endif //FINDSONGDUPLICATES_SEARCHQUERYDIFFERENCE_H
