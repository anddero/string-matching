//
// Created by andde on 07-Jan-21.
//

#ifndef FINDSONGDUPLICATES_WORDDIFF_H
#define FINDSONGDUPLICATES_WORDDIFF_H

#include <vector>
#include <string>
#include "Move.h"

void replace(std::string& src, const std::string& what, const std::string& with);

std::vector<Move> get_diff(const std::string&, const std::string&);

#endif //FINDSONGDUPLICATES_WORDDIFF_H
