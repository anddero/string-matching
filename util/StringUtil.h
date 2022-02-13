//
// Created by karlandero on 20.01.21.
//

#ifndef FINDSONGDUPLICATES_STRINGUTIL_H
#define FINDSONGDUPLICATES_STRINGUTIL_H

#include <string>
#include <vector>

namespace Util {

    class StringUtil { // TODO Test all methods

    public:
        static std::string repeat(const std::string &s, int count);
        static bool contains(const std::string &s, char c);
        static std::string lowercase(std::string s);
        static std::vector<std::string> split(const std::string &splittable, const std::string &letters);
        static std::string filter(const std::string &s, const std::string &letters);
        static bool is_ascii(const std::string &s);
        static std::string join(const std::vector<std::string> &v, const std::string &s = "");
        static std::string pad_right(const std::string &phrase_to_pad, char pad_char, unsigned result_min_len);
    };

}


#endif //FINDSONGDUPLICATES_STRINGUTIL_H
