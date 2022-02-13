//
// Created by karlandero on 21.01.21.
//

#ifndef FINDSONGDUPLICATES_SEARCHQUERYDIFFERENCE_H
#define FINDSONGDUPLICATES_SEARCHQUERYDIFFERENCE_H


#include <string>

std::string normalize_phrase(const std::string &phrase);

unsigned search_query_difference(const std::string &, const std::string &);

float search_query_similarity(const std::string &, const std::string &);


#endif //FINDSONGDUPLICATES_SEARCHQUERYDIFFERENCE_H
