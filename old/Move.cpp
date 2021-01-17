//
// Created by andde on 07-Jan-21.
//

#include <stdexcept>
#include "Move.h"

Move::Move(unsigned long long int score_after_move, Move::Operation operation, bool part_of_complex,
           bool end_of_complex, unsigned long long int index_first, unsigned long long int index_second)
        : score_after_move(score_after_move), operation(operation), part_of_complex(part_of_complex),
          end_of_complex(end_of_complex), index_first(index_first), index_second(index_second) {}

Move::Move() {}

bool Move::operator==(const Move &rhs) const {
    return score_after_move == rhs.score_after_move &&
           operation == rhs.operation &&
           part_of_complex == rhs.part_of_complex &&
           end_of_complex == rhs.end_of_complex &&
           index_first == rhs.index_first &&
           index_second == rhs.index_second;
}

bool Move::operator!=(const Move &rhs) const {
    return !(*this == rhs);
}

std::ostream &operator<<(std::ostream &os, const Move &move) {
    os << "score_after_move: " << move.score_after_move << " operation: " << move.operation << " part_of_complex: "
       << move.part_of_complex << " end_of_complex: " << move.end_of_complex << " index_first: " << move.index_first
       << " index_second: " << move.index_second;
    return os;
}

std::ostream &operator<<(std::ostream &os, const Move::Operation &op) {
    os << to_string(op);
    return os;
}

std::string to_string(const Move::Operation& operation) {
    switch (operation) {
        case Move::Operation::Remove:
            return "Remove";
        case Move::Operation::Insert:
            return "Insert";
        case Move::Operation::Skip:
            return "Skip";
        default:
            throw std::logic_error("Unhandled operation: " + operation);
    }
}

std::string to_string(const Move& move) {
    return to_string(move.operation) + " " + std::to_string(move.index_first) + " " + std::to_string(move.index_second);
}
