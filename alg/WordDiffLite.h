//
// Created by andde on 11-Jan-21.
//

#ifndef FINDSONGDUPLICATES_WORDDIFFLITE_H
#define FINDSONGDUPLICATES_WORDDIFFLITE_H

#include <string>
#include <vector>
#include "Operation.h"
#include "Difference.h"
#include "Cell.h"

// TODO Checked, Improve algorithm, test everything, then optimize

Difference calculate_diff(const std::string &word1, const std::string &word2);

Cell calculate_next_cell(const Cell *ver, const Cell *diag, const Cell *hor, bool equal);

#endif //FINDSONGDUPLICATES_WORDDIFFLITE_H
