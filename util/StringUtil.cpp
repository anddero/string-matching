//
// Created by karlandero on 20.01.21.
//

#include "StringUtil.h"

std::string Util::StringUtil::repeat(const std::string &s, int count) {
    if (count <= 0) return "";
    if (count == 1) return s;
    if (count == 2) return s + s;
    if (count == 3) return s + s + s;
    std::string half = repeat(s, count / 2);
    std::string even_part = half + half;
    if (count % 2) {
        return even_part + s;
    }
    return even_part;
}
