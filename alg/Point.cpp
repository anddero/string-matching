//
// Created by andde on 11-Jan-21.
//

#include <cmath>
#include "Point.h"

Point::Point(unsigned long long int closed_groups, long long int open_group_diff, Point::Source source,
             unsigned long long int source_point_index) : closed_groups(closed_groups),
                                                          open_group_diff(open_group_diff), source(source),
                                                          source_point_index(source_point_index) {}

bool Point::operator<=(const Point &other) const {
    return other.closed_groups >= closed_groups
           && (other.closed_groups - closed_groups) >= std::abs(other.open_group_diff - open_group_diff);
}

Point Point::move_hor(unsigned long long source_point_index) const {
    return Point(closed_groups, open_group_diff + 1, Hor, source_point_index);
}

Point Point::move_ver(unsigned long long source_point_index) const {
    return Point(closed_groups, open_group_diff - 1, Ver, source_point_index);
}

Point Point::move_diag(unsigned long long source_point_index) const {
    return Point(get_expansion(), 0, Diag, source_point_index);
}

unsigned long long Point::get_expansion() const {
    return closed_groups + std::abs(open_group_diff);
}
