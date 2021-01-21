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

bool Util::StringUtil::contains(const std::string &s, char c) {
    for (char sc : s) {
        if (sc == c) return true;
    }
    return false;
}

std::string Util::StringUtil::lowercase(std::string s) {
    for (char &c : s) {
        c = (char) std::tolower((unsigned char) c);
    }
    return s;
}

std::vector<std::string> Util::StringUtil::split(const std::string &splittable, const std::string &letters) {
    std::vector<std::string> substrings;
    unsigned start = 0;
    for (unsigned i = 0; i != splittable.size(); ++i) {
        if (contains(letters, splittable[i])) {
            substrings.push_back(splittable.substr(start, i - start));
            start = i + 1;
        }
    }
    substrings.push_back(splittable.substr(start));
    return substrings;
}

std::string Util::StringUtil::filter(const std::string &s, const std::string &letters) {
    std::string sn;
    for (unsigned i = 0; i != s.size(); ++i) {
        if (!contains(letters, s[i])) sn.push_back(s[i]);
    }
    return sn;
}
