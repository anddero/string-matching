//
// Created by karlandero on 20.01.21.
//

#ifndef FINDSONGDUPLICATES_STRINGUTIL_H
#define FINDSONGDUPLICATES_STRINGUTIL_H

#include <string>
#include <vector>

namespace Util {

    class StringUtil {

    public:
        static std::string repeat(const std::string &s, int count); // TODO Test
        static bool contains(const std::string &s, char c); // TODO Test
        static std::string lowercase(std::string s); // TODO Test
        static std::vector<std::string> split(const std::string &splittable, const std::string &letters); // TODO Test
        static std::string filter(const std::string &s, const std::string &letters); // TODO Test
        static bool is_ascii(const std::string &s); // TODO Test
    };

}


#endif //FINDSONGDUPLICATES_STRINGUTIL_H
