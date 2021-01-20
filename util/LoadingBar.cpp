//
// Created by karlandero on 20.01.21.
//

#include <iostream>
#include "LoadingBar.h"
#include "StringUtil.h"

Util::LoadingBar::LoadingBar(int total) : total{total} {}

void Util::LoadingBar::conditional_display(int index, const std::string &prefix, const std::string &postfix) {
    float perc = total <= 0 ? 1.f : index / (float) total;

    auto bar_length = static_cast<int>(total_bar_length * perc);
    if (bar_length > prev_bar_length) {
        prev_bar_length = bar_length;
        int space_count = total_bar_length - bar_length;
        std::cout << prefix
                  << "[" << StringUtil::repeat("=", bar_length) << StringUtil::repeat(" ", space_count) << "] "
                  << std::to_string((int) (perc * 100)) << "%"
                  << postfix;
    }
}
