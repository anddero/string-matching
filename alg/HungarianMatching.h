//
// Created by karlandero on 24.01.21.
//

#ifndef FINDSONGDUPLICATES_HUNGARIANMATCHING_H
#define FINDSONGDUPLICATES_HUNGARIANMATCHING_H


class HungarianMatching {
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

    HungarianMatching(unsigned const * src_matrix, unsigned dim);

    virtual ~HungarianMatching();

    unsigned calculate();
};


#endif //FINDSONGDUPLICATES_HUNGARIANMATCHING_H
