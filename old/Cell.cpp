//
// Created by andde on 11-Jan-21.
//

#include <vector>
#include <stdexcept>
#include "Cell.h"

Cell::Cell(unsigned long long int match_count, bool match) : match_count{match_count},
                                                             match{match} {}

void Cell::add_all_exclusively(const Cell &cell, Point::Source source) {
    for (unsigned long long i = 0; i != cell.points.size(); ++i) {
        switch (source) {
            case Point::Ver:
                add_exclusively(cell.points[i].move_ver(i));
                break;
            case Point::Hor:
                add_exclusively(cell.points[i].move_hor(i));
                break;
            case Point::Diag:
                add_exclusively(cell.points[i].move_diag(i));
                break;
            default:
                throw std::logic_error("Unhandled source direction: " + std::to_string(source));
        }
    }
}

Point Cell::reduce_by_expansion() const {
    const Point *best_p = &points[0];
    auto best_exp = best_p->get_expansion();
    const Point *p;
    typeof(best_exp) p_exp;
    for (unsigned long long i = 1; i != points.size(); ++i) {
        p = &points[i];
        p_exp = p->get_expansion();
        if (p_exp < best_exp) {
            best_p = p;
            best_exp = p_exp;
        }
    }
    return *best_p;
}

void Cell::add_exclusively(const Point &point) {
    for (unsigned long long i = 0; i != points.size(); ++i) {
        if (points[i] <= point) {
            return;
        }
        if (point <= points[i]) {
            points.erase(points.begin() + i--);
            continue;
        }
    }
    points.push_back(point);
}
