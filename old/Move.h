//
// Created by andde on 07-Jan-21.
//

#ifndef FINDSONGDUPLICATES_MOVE_H
#define FINDSONGDUPLICATES_MOVE_H

#include <string>
#include <vector>
#include <ostream>

struct Move {
    enum Operation {
        Remove,
        Insert,
        Skip
    };

    unsigned long long score_after_move;
    Operation operation;
    bool part_of_complex;
    bool end_of_complex;
    unsigned long long index_first;
    unsigned long long index_second;

    Move();

    Move(unsigned long long int score_after_move, Operation operation, bool part_of_complex, bool end_of_complex,
         unsigned long long int index_first, unsigned long long int index_second);

    bool operator==(const Move &rhs) const;

    bool operator!=(const Move &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const Move &move);

    friend std::ostream &operator<<(std::ostream &os, const Operation &op);
};

std::string to_string(const Move::Operation&);

std::string to_string(const Move&);

#endif //FINDSONGDUPLICATES_MOVE_H
