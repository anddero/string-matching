//
// Created by karlandero on 24.01.21.
//

#ifndef FINDSONGDUPLICATES_HUNGARIANMATCHING_H
#define FINDSONGDUPLICATES_HUNGARIANMATCHING_H

/**
 * Implementation of the Munkres' Assignment Algorithm based on the article
 * from https://brc2.com/the-algorithm-workshop/
 */
class HungarianMatching {

    // Output utils
    void output_src_matrix();
    void output_stars_matrix();

    // Source
    unsigned const * const src_matrix;
    unsigned const dim;

    // Calculation progress
    unsigned* matrix;
    bool* stars;
    bool* primes;
    bool* col_marks;
    bool* row_marks;
    unsigned backtrack_remembered_row;
    unsigned backtrack_remembered_col;

    // Helpers to obtain matrix elements
    inline const unsigned& el_src(unsigned row, unsigned col) const;
    inline unsigned& el(unsigned row, unsigned col);
    inline bool& star(unsigned row, unsigned col);
    inline bool& prime(unsigned row, unsigned col);

    // Algorithm steps
    bool outer_loop_iteration(); // Return true if outer_loop_iteration should be called again, false to finish
    bool inner_loop_iteration(); // Return true if inner_loop_iteration should be called again, false to finish
    bool backtrack_loop_iteration(); // Return true if the backtrack loop iteration should be called again, false to finish
public:

    struct Result {
        unsigned difference;
        /**
         * All matrices (including the constructor input matrix) are treated such that the rows are appended to
         * each-other. For example, the input vector [0, 1, 2, 3, 4, 5, 6, 7, 8] is treated as the following matrix:
         * 0 1 1
         * 3 8 5
         * 2 7 4
         * If the 'matching_cols_by_row' property has the content [1, 2, 0], then by the above example matrix, the
         * matches would be (shown as asterisks):
         * - * -
         * - - *
         * * - -
         * Thus, as the property name says, you get the column index if you ask for an element by the row index:
         * matching_cols_by_row[1] = 2 // row = 1, col = 2
         * And the 'difference' value would accordingly have to be 1+5+2 = 8
         */
        std::vector<unsigned> matching_cols_by_row;

        Result(unsigned int score, const std::vector<unsigned int> &matchingColsByRow)
                : difference(score)
                , matching_cols_by_row(matchingColsByRow) {}
    };

    HungarianMatching(unsigned const * src_matrix, unsigned dim);

    virtual ~HungarianMatching();

    Result calculate();
};


#endif //FINDSONGDUPLICATES_HUNGARIANMATCHING_H
