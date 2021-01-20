//
// Created by andde on 11-Jan-21.
//

#ifndef FINDSONGDUPLICATES_POINT_H
#define FINDSONGDUPLICATES_POINT_H

struct Point {
    enum Source {
        Ver,
        Hor,
        Diag
    };

    unsigned long long closed_groups;
    long long open_group_diff;
    Source source;
    unsigned long long source_point_index;

    Point(unsigned long long int closed_groups, long long int open_group_diff, Source source,
          unsigned long long int source_point_index);

    bool operator<=(const Point&) const;
    Point move_hor(unsigned long long source_point_index) const;
    Point move_ver(unsigned long long source_point_index) const;
    Point move_diag(unsigned long long source_point_index) const;
    unsigned long long get_expansion() const;
};

#endif //FINDSONGDUPLICATES_POINT_H
