//
// Created by karlandero on 21.01.21.
//

#include <stdexcept>
#include <vector>
#include <set>
#include <algorithm>
#include "MinimumLinearAssignment.h"

const unsigned MAX_UNSIGNED = std::numeric_limits<unsigned>::max();

struct Value {
    unsigned value;
    unsigned row;

    Value(unsigned int value, unsigned int row) : value(value), row(row) {}

    bool operator<(const Value &rhs) const {
        return value < rhs.value;
    }
};

inline unsigned el(const unsigned matrix[], const unsigned dim, const unsigned row, const unsigned col) {
    return matrix[row * dim + col];
}

inline unsigned calc_max_next(const unsigned col_mins[],
                              const unsigned dim, 
                              const unsigned next_col_index,
                              const unsigned min_score,
                              const unsigned cur_score
) {
    const unsigned col_min_index = next_col_index + 1;
    const unsigned subt_term = cur_score + (col_min_index < dim ? col_mins[col_min_index] : 0);
    return subt_term > min_score ? 0 : min_score - subt_term;
}

void min_assign_score(const unsigned matrix[], // TODO Presort all columns before calling
                      const unsigned col_mins[],
                      unsigned &min_score,
                      const unsigned dim,
                      const std::set<unsigned> &used_row_indices,
                      unsigned max_next,
                      const unsigned score) {
    const auto col = (unsigned) used_row_indices.size();
    if (col == dim) {
        if (score < min_score) {
            min_score = score;
        }
        return;
    }

    std::vector<Value> values;
    for (unsigned row = 0; row != dim; ++row) {
        if (used_row_indices.find(row) != used_row_indices.end()) continue;
        unsigned value = el(matrix, dim, row, col);
        if (value >= max_next) continue;
        values.emplace_back(value, row);
    }
    std::sort(values.begin(), values.end());
    unsigned prev_min_score = min_score;
    for (const auto& value : values) {
        if (min_score != prev_min_score) {
            unsigned diff = prev_min_score - min_score;
            if (diff > max_next) return;
            prev_min_score = min_score;
            max_next -= diff;
        }
        if (value.value >= max_next) return;
        auto used_row_indices_new = used_row_indices;
        used_row_indices_new.insert(value.row);
        min_assign_score(
                matrix,
                col_mins,
                min_score,
                dim,
                used_row_indices_new,
                calc_max_next(col_mins, dim, col + 1, min_score, score + value.value),
                score + value.value
        );
    }
}

unsigned min_assign_score(const unsigned matrix[], const unsigned dim) { // TODO Test
    if (dim == 0) {
        throw std::runtime_error("Expected at least one element in matrix");
    }
    if (dim == 1) {
        return matrix[0];
    }
    if (dim > 100) {
        throw std::runtime_error("This linear assignment algorithm is intended for use with small task sizes");
    }
    
    unsigned min_score = 0;
    unsigned col_mins[dim];
    for (unsigned i = 0; i != dim; ++i) {
        unsigned col = dim - i - 1;
        unsigned cur_col_min = el(matrix, dim, 0, col);
        for (unsigned row = 1; row != dim; ++row) {
            cur_col_min = std::min(cur_col_min, el(matrix, dim, row, col));
        }
        col_mins[col] = cur_col_min + (col + 1 < dim ? col_mins[col + 1] : 0);
        min_score += el(matrix, dim, col, col);
    }
    if (min_score == col_mins[0]) return min_score;
    
    min_assign_score(
            matrix,
            col_mins, 
            min_score, 
            dim, 
            {}, 
            calc_max_next(col_mins, dim, 0, min_score, 0),
            0
    );
    
    return min_score;
}
