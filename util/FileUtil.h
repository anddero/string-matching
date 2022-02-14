//
// Created by andde on 2/8/2022.
//

#ifndef STRINGMATCHING_FILEUTIL_H
#define STRINGMATCHING_FILEUTIL_H

#include <string>
#include <vector>

namespace Util {

    class FileUtil {

    public:
        static std::vector<std::string> readSourceFile(const std::string &); // TODO Test
    };

}

#endif //STRINGMATCHING_FILEUTIL_H
