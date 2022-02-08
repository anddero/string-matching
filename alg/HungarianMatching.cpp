//
// Created by karlandero on 24.01.21.
//

#include <stdexcept>
#include <limits>
#include "HungarianMatching.h"

HungarianMatching::HungarianMatching(unsigned const * const src_matrix, const unsigned dim)
        : src_matrix{src_matrix}
        , dim{dim} {
    // Special case checks
    if (dim == 0) {
        throw std::runtime_error("Expected at least one element in matrix");
    }
    if (dim > 1000) {
        throw std::runtime_error("This algorithm may have polynomial complexity of up to n^4,"
                                 " matrix size limited to 1000*1000");
    }

    // Initialize required memory
    matrix = new unsigned[dim * dim]{};
    stars = new bool[dim * dim]{false};
    primes = new bool[dim * dim]{false};
    col_marks = new bool[dim]{false};
    row_marks = new bool[dim]{false};
}

HungarianMatching::~HungarianMatching() {
    delete matrix;
    delete stars;
    delete primes;
    delete col_marks;
    delete row_marks;
}

inline const unsigned& HungarianMatching::el_src(const unsigned row, const unsigned col) const {
    return src_matrix[row * dim + col];
}

inline unsigned& HungarianMatching::el(const unsigned row, const unsigned col) {
    return matrix[row * dim + col];
}

bool &HungarianMatching::star(unsigned row, unsigned col) {
    return stars[row * dim + col];
}

bool &HungarianMatching::prime(unsigned row, unsigned col) {
    return primes[row * dim + col];
}

bool HungarianMatching::outer_loop_iteration() {
    // Mark columns with a star
    unsigned stars_count = 0;
    for (unsigned col = 0; col != dim; ++col) {
        for (unsigned row = 0; row != dim; ++row) {
            if (star(row, col)) { // Found a star
                col_marks[col] = true; // Mark the column
                ++stars_count;
                break; // Continue to next column (no more stars can be here)
            }
        }
    }

    // Check if all columns are marked
    bool all_columns_marked = stars_count == dim;

    // If all columns are marked, end the algorithm
    if (all_columns_marked) {
        return false;
    }

    // Inner-loop of the algorithm (priming un-starred zeroes, back-tracking, alternations)
    while (inner_loop_iteration());
    return true;
}

bool HungarianMatching::inner_loop_iteration() {
    // Find un-starred 0 amongst unmarked columns and rows
    bool found = false;
    unsigned row = 0, col = 0; // After breaking loops, will hold position of found 0
    for (col = 0; col != dim; ++col) {
        if (col_marks[col]) { // If the column is marked, no reason to look in
            continue; // Continue to next column
        }
        for (row = 0; row != dim; ++row) {
            if (el(row, col) == 0 && !star(row, col) && !row_marks[row]) { // Un-starred and unmarked 0
                found = true; // Found it
                break; // Do not iterate any further, so that row and col are accurate
            }
        }
        if (found) { // Found, do not iterate any further, so that row and col are accurate
            break;
        }
    }

    if (found) {
        // Mark the found 0 as prime
        prime(row, col) = true;

        // Remember it
        backtrack_remembered_row = row;
        backtrack_remembered_col = col;

        // Find star in same row
        bool star_found = false;
        for (col = 0; col != dim; ++col) {
            if (star(backtrack_remembered_row, col)) { // Star found
                star_found = true;
                break; // Do not iterate any further, so that col holds the correct value
            }
        }

        if (star_found) {
            // Mark row
            row_marks[backtrack_remembered_row] = true;

            // Unmark star column
            col_marks[col] = false;

            // Continue with inner-loop
            return true;

        }

        // Star not found in the row of the new prime

        // Backtrack-loop based on remembered 0 (starts with remembered prime)
        while (backtrack_loop_iteration());

        // Unmark all rows and remove all primes, getting ready for another outer-loop iteration
        // Note: column marks are kept through-out the entire algorithm live, to reduce excess operations
        for (row = 0; row != dim; ++row) {
            for (col = 0; col != dim; ++col) {
                prime(row, col) = false; // Remove prime
            }
            row_marks[row] = false; // Remove row mark
        }

        // Unmark all columns
        for (col = 0; col != dim; ++col) {
            col_marks[col] = false;
        }

        // Continue with outer-loop (break inner-loop)
        return false;

    }

    // Un-starred 0 not found amongst unmarked cells

    // Find smallest value amongst unmarked rows and unmarked columns
    unsigned smallest = std::numeric_limits<unsigned>::max();
    for (row = 0; row != dim; ++row) {
        if (row_marks[row]) { // If row is marked, continue to next row
            continue;
        }
        for (col = 0; col != dim; ++col) {
            if (!col_marks[col]) { // Column not marked
                smallest = std::min(smallest, el(row, col));
            }
        }
    }

    // Add the value to marked rows
    for (row = 0; row != dim; ++row) {
        if (!row_marks[row]) { // If row is not marked, continue to next row
            continue;
        }
        // Row is marked, add smallest unmarked value to all values of the row
        for (col = 0; col != dim; ++col) {
            el(row, col) += smallest;
        }
    }

    // Subtract the value from unmarked columns
    for (col = 0; col != dim; ++col) {
        if (col_marks[col]) { // Column is marked, continue to next column
            continue;
        }
        // Column is not marked, subtract smallest unmarked value from all values of the column
        for (row = 0; row != dim; ++row) {
            el(row, col) -= smallest;
        }
    }

    // Continue with inner-loop
    return true;
}

bool HungarianMatching::backtrack_loop_iteration() {
    // Check whether last remembered element is prime or star and choose appropriate action
    if (prime(backtrack_remembered_row, backtrack_remembered_col)) { // Is prime
        // Discard the prime
        prime(backtrack_remembered_row, backtrack_remembered_col) = false;

        // Mark as star
        star(backtrack_remembered_row, backtrack_remembered_col) = true;

        // Mark column
        col_marks[backtrack_remembered_col] = true;

        // Search for star in the same column
        bool found_star = false;
        unsigned row = 0;
        for (row = 0; row != dim; ++row) {
            if (star(row, backtrack_remembered_col) && row != backtrack_remembered_row) { // Star found
                found_star = true;
                break; // Do not iterate any further, such that row has the correct value
            }
        }

        if (found_star) {
            // Remember the star for next iteration of backtrack loop
            backtrack_remembered_row = row;
            return true;
        }

        // Star not found, break backtrack-loop
        return false;
    }

    // Remembered cell is a star

    // Discard star (no need to unmark column, always contained a prime before)
    star(backtrack_remembered_row, backtrack_remembered_col) = false;

    // Find the prime in the same row, always exists
    for (backtrack_remembered_col = 0; backtrack_remembered_col != dim; ++backtrack_remembered_col) {
        if (prime(backtrack_remembered_row, backtrack_remembered_col)) { // Prime found
            break; // Do not iterate any further, prime is now remembered
        }
    }

    // Continue to next backtracking iteration with the found prime
    return true;
}

unsigned HungarianMatching::calculate() {
    // 1x1 matrix
    if (dim == 1) {
        return src_matrix[0];
    }

    // Copy matrix
    for (unsigned i = 0; i != dim * dim; ++i) {
        matrix[i] = src_matrix[i];
    }

    // Normalize rows, such that each row contains at least one zero
    for (unsigned row = 0; row != dim; ++row) {
        unsigned row_min = el(row, 0);
        for (unsigned col = 1; col != dim; ++col) {
            if (row_min == 0) break;
            row_min = std::min(row_min, el(row, col));
        }
        if (row_min == 0) continue;
        for (unsigned col = 0; col != dim; ++col) {
            el(row, col) -= row_min;
        }
    }

    // Normalize columns, such that each column contains at least one zero
    for (unsigned col = 0; col != dim; ++col) {
        unsigned col_min = el(0, col);
        for (unsigned row = 1; row != dim; ++row) {
            if (col_min == 0) break;
            col_min = std::min(col_min, el(row, col));
        }
        if (col_min == 0) continue;
        for (unsigned row = 0; row != dim; ++row) {
            el(row, col) -= col_min;
        }
    }

    // Mark initial stars, mark columns in the process
    for (unsigned row = 0; row != dim; ++row) {
        // Find one unmarked zero per row, mark the star and column, then continue to next row
        for (unsigned col = 0; col != dim; ++col) {
            if (el(row, col) == 0 && !col_marks[col]) { // Unmarked zero
                star(row, col) = true; // Mark the star
                col_marks[col] = true; // Mark the column
                break; // Continue to next row
            }
        }
    }

    // Outer loop of the algorithm
    while (outer_loop_iteration());

    // Calculate sum of starred cells from original matrix
    unsigned sum = 0;
    for (unsigned row = 0; row != dim; ++row) {
        // Find one star per row, then continue to next row
        for (unsigned col = 0; col != dim; ++col) {
            if (star(row, col)) { // Star present
                sum += el_src(row, col); // Add same cell from source
                break; // Continue to next row
            }
        }
    }

    // Return result
    return sum;
}
