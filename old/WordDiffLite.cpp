//
// Created by andde on 11-Jan-21.
//

#include <stdexcept>
#include "WordDiffLite.h"
#include "Cell.h"

Difference calculate_diff(const std::string &hor_word, const std::string &ver_word) {
    if (hor_word.length() > 100 || ver_word.length() > 100) {
        throw std::logic_error("Words of large sizes not recommended to be compared by this algorithm due to sequential memory");
    }

    // Declare matrix and its sizes
    const unsigned long long col_count = hor_word.length() + 1;
    const unsigned long long row_count = ver_word.length() + 1;
    Cell matrix[row_count][col_count];

    // Forward propagation (calculate cell values)
    // TODO The entire algorithm might be improvable by minimizing the term (expansion_size / 2 - match_count)
    for (unsigned long long row_i = 0; row_i != row_count; ++row_i) {
        for (unsigned long long col_i = 0; col_i != col_count; ++col_i) {
            bool equal = row_i != ver_word.length() && col_i != hor_word.length() && ver_word[row_i] == hor_word[col_i];
            if (row_i == 0 && col_i == 0) {
                matrix[row_i][col_i] = Cell(0, equal);
                matrix[row_i][col_i].points.emplace_back(0, 0, Point::Diag, 0);
                continue;
            }
            matrix[row_i][col_i] = calculate_next_cell(
                    row_i == 0 ? nullptr : &matrix[row_i - 1][col_i],
                    row_i == 0 || col_i == 0 ? nullptr : &matrix[row_i - 1][col_i - 1],
                    col_i == 0 ? nullptr : &matrix[row_i][col_i - 1],
                    equal
            );
        }
    }

    // Backward propagation (backtrack to collect min diff)
    auto row_i = row_count - 1;
    auto col_i = col_count - 1;
    auto* cell = &matrix[row_i][col_i];
    auto* point = &cell->points[0];
    Point last_point = cell->reduce_by_expansion();
    Difference difference(hor_word.length(), ver_word.length(), cell->match_count, last_point.get_expansion());

    auto del_count = 0ull;
    auto ins_count = 0ull;

    while (true) {
        switch (point->source) {
            case Point::Hor:
                --col_i;
                ++del_count;
                break;
            case Point::Diag:
                if (del_count + ins_count > 0) {
                    if (del_count > ins_count) {
                        difference.operations.insert(difference.operations.begin(), Operation(Operation::DELETE, col_i + ins_count, row_i + ins_count, del_count - ins_count));
                        if (ins_count != 0) {
                            difference.operations.insert(difference.operations.begin(), Operation(Operation::REPLACE, col_i, row_i, ins_count));
                        }
                    } else if (del_count == ins_count) {
                        difference.operations.insert(difference.operations.begin(), Operation(Operation::REPLACE, col_i, row_i, ins_count));
                    } else {
                        difference.operations.insert(difference.operations.begin(), Operation(Operation::INSERT, col_i + del_count, row_i + del_count, ins_count - del_count));
                        if (del_count != 0) {
                            difference.operations.insert(difference.operations.begin(), Operation(Operation::REPLACE, col_i, row_i, del_count));
                        }
                    }
                    del_count = 0ull;
                    ins_count = 0ull;
                }
                if (row_i == 0 && col_i == 0) {
                    return difference;
                }
                --row_i;
                --col_i;
                break;
            case Point::Ver:
                --row_i;
                ++ins_count;
                break;
            default:
                throw std::logic_error("Unhandled point source: " + std::to_string(point->source));
        }
        cell = &matrix[row_i][col_i];
        point = &cell->points[point->source_point_index];
    };
}

Cell calculate_next_cell(const Cell *ver, const Cell *diag, const Cell *hor, const bool equal) {
    unsigned long long max_match_count = 0;
    
    // Calculate maximum match count and only keep cells which should be considered
    if (ver) {
        if (ver->points.empty()) {
            ver = nullptr;
        } else {
            max_match_count = ver->match_count;
        }
    }
    if (diag) {
        unsigned long long diag_match_count = diag->match_count + 1;
        if (!diag->match || diag->points.empty() || diag_match_count < max_match_count) {
            diag = nullptr;
        } else if (max_match_count < diag_match_count) {
            ver = nullptr;
            max_match_count = diag_match_count;
        }
    }
    if (hor) {
        unsigned long long hor_match_count = hor->match_count;
        if (hor->points.empty() || hor_match_count < max_match_count) {
            hor = nullptr;
        } else if (max_match_count < hor_match_count) {
            ver = nullptr;
            diag = nullptr;
            max_match_count = hor_match_count;
        }
    }
    
    // Set up new cell with initial values
    Cell cell(max_match_count, equal);
    
    // Populate cell with new points
    if (ver) cell.add_all_exclusively(*ver, Point::Ver);
    if (diag) cell.add_all_exclusively(*diag, Point::Diag);
    if (hor) cell.add_all_exclusively(*hor, Point::Hor);

    // Return result
    return cell;
}
