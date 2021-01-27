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

inline unsigned& el(unsigned *matrix, const unsigned dim, const unsigned row, const unsigned col) {
    return matrix[row * dim + col];
}

inline unsigned calc_max_next(const unsigned min_score, const unsigned cur_score) {
    return cur_score >= min_score ? 0 : min_score - cur_score;
}

void min_assign_score(const unsigned matrix[], // TODO Presort all columns before calling
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
                min_score,
                dim,
                used_row_indices_new,
                calc_max_next(min_score, score + value.value),
                score + value.value
        );
    }
}

unsigned min_assign_score(const unsigned matrix_const[], const unsigned dim) { // TODO Test
    if (dim == 0) {
        throw std::runtime_error("Expected at least one element in matrix");
    }
    if (dim == 1) {
        return matrix_const[0];
    }
    if (dim > 100) {
        throw std::runtime_error("This linear assignment algorithm is intended for use with small task sizes");
    }

    // copy matrix
    unsigned matrix[dim * dim];
    for (unsigned i = 0; i != dim * dim; ++i) {
        matrix[i] = matrix_const[i];
    }

    unsigned normalization_term = 0;

    // normalize rows
    for (unsigned row = 0; row != dim; ++row) {
        unsigned row_min = el(matrix, dim, row, 0);
        for (unsigned col = 1; col != dim; ++col) {
            if (row_min == 0) break;
            row_min = std::min(row_min, el(matrix, dim, row, col));
        }
        if (row_min == 0) continue;
        for (unsigned col = 0; col != dim; ++col) {
            el(matrix, dim, row, col) -= row_min;
        }
        normalization_term += row_min;
    }

    // normalize columns
    for (unsigned col = 0; col != dim; ++col) {
        unsigned col_min = el(matrix, dim, 0, col);
        for (unsigned row = 1; row != dim; ++row) {
            if (col_min == 0) break;
            col_min = std::min(col_min, el(matrix, dim, row, col));
        }
        if (col_min == 0) continue;
        for (unsigned row = 0; row != dim; ++row) {
            el(matrix, dim, row, col) -= col_min;
        }
        normalization_term += col_min;
    }

    // find current minimum score by main diagonal
    unsigned min_score = 0;
    for (unsigned i = 0; i != dim; ++i) {
        unsigned main_diag_cell = dim - i - 1;
        min_score += el(matrix, dim, main_diag_cell, main_diag_cell);
    }
    if (min_score == 0) return normalization_term;
    
    min_assign_score(
            matrix,
            min_score, 
            dim, 
            {}, 
            min_score,
            0
    );
    return min_score + normalization_term;
}
