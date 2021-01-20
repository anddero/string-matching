//
// Created by andde on 11-Jan-21.
//

#include "Operation.h"

Operation::Operation(const Operation::Type type,
                     const unsigned long long int index_one,
                     const unsigned long long int index_two,
                     const unsigned long long int length)
        : type{type},
          index_one{index_one},
          index_two{index_two},
          length{length} {}

bool Operation::operator==(const Operation &rhs) const {
    return type == rhs.type &&
           index_one == rhs.index_one &&
           index_two == rhs.index_two &&
           length == rhs.length;
}

bool Operation::operator!=(const Operation &rhs) const {
    return !(rhs == *this);
}
