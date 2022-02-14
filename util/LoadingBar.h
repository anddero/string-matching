//
// Created by karlandero on 20.01.21.
//

#ifndef STRINGMATCHING_LOADINGBARYO_H
#define STRINGMATCHING_LOADINGBARYO_H

#include <string>

namespace Util {

    class LoadingBar {

        static const int total_bar_length = 20;
        const int total;
        int prev_bar_length = 0;

    public:
        explicit LoadingBar(int total);

        void conditional_display(int index, const std::string &prefix = "\t", const std::string &postfix = "\n");
    };

}

#endif //STRINGMATCHING_LOADINGBARYO_H
