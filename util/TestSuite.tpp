//
// Created by karlandero on 20.01.21.
//

#include <iostream>
#include "TestSuite.h"

void Util::TestSuite::write_prefix(const std::string &description) {
    std::cerr << "Failed: " << (pass_count + 1) << ": " << description;
}

Util::TestSuite::TestSuite(std::string name, std::string fin_text) :
        name{std::move(name)},
        fin_text{std::move(fin_text)} {
    std::cout << "Starting test suite: " << this->name << "...";
}

int Util::TestSuite::get_pass_count() const {
    return pass_count;
}

void Util::TestSuite::check(const bool pass, const std::string &description) {
    if (!pass) {
        write_prefix(description);
        std::cerr << std::endl;
        exit(1);
    }
    ++pass_count;
}

template<typename T>
void Util::TestSuite::check(const T actual, const T expected, const std::string &description) {
    if (expected == actual) {
        ++pass_count;
        return;
    }
    write_prefix(description);
    std::cerr << "\n\t" << "Expected: " << expected
         << "\n\t" << "Actual: " << actual
         << "\n";
    exit(1);
}

template<typename T>
void
Util::TestSuite::check(const T actual, const T expected, const T expected_diff, const std::string &description) {
    if (expected == actual) {
        ++pass_count;
        return;
    }
    T actual_diff;
    if (expected > actual) {
        actual_diff = expected - actual;
    } else {
        actual_diff = actual - expected;
    }
    if (actual_diff > expected_diff) {
        write_prefix(description);
        std::cerr << "\n\t" << "Expected: " << expected
             << "\n\t" << "Actual: " << actual
             << "\n\t" << "Diff: " << actual_diff << " > " << expected_diff
             << "\n";
        exit(1);
    }
    ++pass_count;
}

template<typename T>
void Util::TestSuite::check(const std::vector<T> &actual, const std::vector<T> &expected, const std::string &description) {
    if (actual.size() != expected.size()) {
        write_prefix(description);
        std::cerr << "\n\t" << "Actual array has size " << actual.size() << ", but expected " << expected.size() << "\n";
        exit(1);
    }
    auto size = actual.size();
    for (auto i = 0ull; i != size; ++i) {
        if (actual[i] != expected[i]) {
            auto min_i = i < 8 ? 0ull : i - 7;
            auto max_i = size < 9 || i > size - 9 ? size : i + 8;
            write_prefix(description);
            std::cerr << "\n\t" << "Arrays differ at element " << i << ":"
                 << "\n\t" << "Displaying elements in range [" << min_i << "; " << max_i << ")"
                 << "\n\t" << "Expected: " << expected[i]
                 << "\n\t" << "Actual: " << actual[i]
                 << "\n\t" << "Expected array: [";
            for (i = min_i; i != max_i; ++i) {
                std::cerr << expected[i] << ((i == max_i - 1) ? "]" : " ");
            }
            std::cerr << "\n\t" << "Actual array: [";
            for (i = min_i; i != max_i; ++i) {
                std::cerr << actual[i] << ((i == max_i - 1) ? "]" : " ");
            }
            std::cerr << "\n";
            exit(1);
        }
    }
    ++pass_count;
}

Util::TestSuite::~TestSuite() {
    std::cout << fin_text << " " << pass_count << std::endl;
}
