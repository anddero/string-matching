//
// Created by andde on 2/8/2022.
//

#include <stdexcept>
#include <fstream>
#include "FileUtil.h"

std::vector<std::string> Util::FileUtil::readSourceFile(const std::string &file_location) {
    std::ifstream input(file_location);
    if (!input.is_open()) throw std::runtime_error("File '" + file_location + "' could not be opened");
    std::vector<std::string> tmp_input;
    for (std::string line; std::getline(input, line); ) {
        if (!line.empty()) tmp_input.push_back(line);
    }
    input.close();
    return tmp_input;
}
