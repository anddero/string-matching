//
// Created by andde on 2/8/2022.
//

#ifndef FINDSONGDUPLICATES_FILEUTIL_H
#define FINDSONGDUPLICATES_FILEUTIL_H

#include <string>
#include <vector>

namespace Util {

    class FileUtil {

    public:
        static std::vector<std::string> readSourceFile(const std::string &); // TODO Test
    };

}

#endif //FINDSONGDUPLICATES_FILEUTIL_H
