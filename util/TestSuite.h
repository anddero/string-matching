//
// Created by karlandero on 20.01.21.
//

#ifndef STRINGMATCHING_TESTSUITE_H
#define STRINGMATCHING_TESTSUITE_H

#include <vector>
#include <string>

#define STRINGMATCHING_TESTSUITE_H_NO_DESCRIPTION "<no description>"

namespace Util {

    class TestSuite {

        const std::string name;
        const std::string fin_text;
        int pass_count = 0;

        void write_prefix(const std::string &description);

    public:
        explicit TestSuite(std::string name, std::string fin_text = "passed all");

        int get_pass_count() const;

        void check(bool pass, const std::string &description = STRINGMATCHING_TESTSUITE_H_NO_DESCRIPTION);

        template<typename T>
        void check(T actual, T expected, const std::string &description = STRINGMATCHING_TESTSUITE_H_NO_DESCRIPTION);

        template<typename T>
        void check(T actual, T expected, T expected_diff, const std::string &description = STRINGMATCHING_TESTSUITE_H_NO_DESCRIPTION);

        template<typename T>
        void check(const std::vector<T> &actual, const std::vector<T> &expected, const std::string &description = STRINGMATCHING_TESTSUITE_H_NO_DESCRIPTION);

        ~TestSuite();
    };

}

#include "TestSuite.tpp"

#endif //STRINGMATCHING_TESTSUITE_H
