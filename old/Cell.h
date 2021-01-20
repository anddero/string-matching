//
// Created by andde on 11-Jan-21.
//

#ifndef FINDSONGDUPLICATES_CELL_H
#define FINDSONGDUPLICATES_CELL_H

#include <vector>
#include "Point.h"

struct Cell { // TODO Checked, Test all methods
    unsigned long long match_count{0};
    bool match{false};
    std::vector<Point> points;

    Cell() = default;
    Cell(unsigned long long int match_count, bool match);
    void add_all_exclusively(const Cell &, Point::Source);
    Point reduce_by_expansion() const;

private:
    void add_exclusively(const Point &);
};

#endif //FINDSONGDUPLICATES_CELL_H
