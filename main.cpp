#include <string>
#include <iostream>
#include "util/TestSuite.h"
#include "alg/Levenshtein.h"
#include "alg/DamerauLevenshtein.h"
#include "util/Random.h"
#include "util/StringUtil.h"
#include "util/LoadingBar.h"
#include "alg/MinimumLinearAssignment.h"

// Tests

void tl(Util::TestSuite& ts, const std::string& w1, const std::string& w2, const unsigned expected_length) {
    ts.check(levenshtein(w1, w2), expected_length);
}

void tdl(Util::TestSuite& ts, const std::string& w1, const std::string& w2, const unsigned expected_length) {
    ts.check(damerau_levenshtein_simplified(w1, w2), expected_length);
}

void tla(Util::TestSuite& ts, const std::vector<unsigned> matrix, const unsigned expected_score) {
    auto dim = (unsigned) lround(sqrt(matrix.size()));
    unsigned matrix_raw[dim * dim];
    for (unsigned i = 0; i != dim * dim; ++i) {
        matrix_raw[i] = matrix[i];
    }
    ts.check(min_assign_score(matrix_raw, dim), expected_score);
}

void test_simple_cases() {
    Util::TestSuite ts("Simple Cases");

    tl(ts, "", "a", 1);
    tl(ts, "a", "a", 0);
    tl(ts, "b", "a", 1);
    tl(ts, "ae", "ea", 2);
    tl(ts, "ee", "ae", 1);
    tl(ts, "baa", "aba", 2);
    tl(ts, "aaa", "aabbaa", 3);

    tdl(ts, "", "a", 1);
    tdl(ts, "a", "a", 0);
    tdl(ts, "b", "a", 1);
    tdl(ts, "ae", "ea", 1);
    tdl(ts, "ee", "ae", 1);
    tdl(ts, "baa", "aba", 1);
    tdl(ts, "aaa", "aabbaa", 3);

    tla(ts, {0, 0, 0, 0}, 0);
    tla(ts, {0, 1, 0, 1}, 1);
    tla(ts, {0, 1, 1, 0}, 0);
    tla(ts, {1, 2, 3, 4}, 5);
    tla(ts, {1, 1, 1, 1}, 2);
    tla(ts, {3, 3, 7, 4}, 7);
    tla(ts, {1, 3, 2, 4}, 5);
    tla(ts, {1, 3, 4, 2}, 3);
    tla(ts, {4, 3, 2, 2}, 5);
    tla(ts, {1, 2, 3, 5}, 5);
}

void test_edge_cases() {
    Util::TestSuite ts("Edge Cases");

    tl(ts, "", "", 0);
    tl(ts, "", "google", 6);

    tdl(ts, "", "", 0);
    tdl(ts, "", "google", 6);

    tla(ts, {0}, 0);
    tla(ts, {1}, 1);
    tla(ts, {4}, 4);
    tla(ts, {7}, 7);
}

void test_complex_cases() {
    Util::TestSuite ts("Complex Cases");

    tl(ts, "abc", "def", 3);
    tl(ts, "abc", "bac", 2);
    tl(ts, "AED", "ADE", 2);
    tl(ts, "bcbd", "cbdb", 2);
    tl(ts, "LIME", "LIEM", 2);
    tl(ts, "Hello", "hekko", 3);
    tl(ts, "EAED", "ADE", 3);
    tl(ts, "AED", "EADE", 3);
    tl(ts, "ab12", "de21", 4);
    tl(ts, "hello", "helllo", 1);
    tl(ts, "vihik", "wihhil", 3);
    tl(ts, "sahtel", "saddle", 4);
    tl(ts, "abcbdeefg", "acbdbeefg", 2);
    tl(ts, "LO WORL", "RO WOR", 2);
    tl(ts, "AECDAED", "ECDEADE", 4);
    tl(ts, "AECDEAED", "ECDADE", 4);
    tl(ts, "telefon", "tekefon", 1);
    tl(ts, "telefon", "tellefon", 1);
    tl(ts, "EBEHMOT", "BEGEMOTH", 4);
    tl(ts, "AAAA BBBB", "AAA BBBB", 1);
    tl(ts, "I WANT BEER", "DEAR", 9);
    tl(ts, "DEAR WORLD", "HELLO WORL", 5);
    tl(ts, "HELLO WORLD", "HELO WORLD", 1);
    tl(ts, "HELLO WORLD", "HELRO WORD", 2);
    tl(ts, "HELLO WORLD", "HELLO DEAR WORLD", 5);
    tl(ts, "what d o um ean", "wjat do u mean", 4);
    tl(ts, "010", "10", 1);
    tl(ts, "010", "101010", 3);
    tl(ts, "010101010", "101010", 3);
    tl(ts, "01010101010010101010", "101001011010010", 5);
    tl(ts, "what d o um ean", "wjhat do you mean jb", 9);

    tdl(ts, "abc", "def", 3);
    tdl(ts, "abc", "bac", 1);
    tdl(ts, "abc", "abcdef", 3);
    tdl(ts, "abcdefghijk", "abcdefhijk", 1);
    tdl(ts, "AED", "ADE", 1);
    tdl(ts, "bcbd", "cbdb", 2);
    tdl(ts, "LIME", "LIEM", 1);
    tdl(ts, "Hello", "hekko", 3);
    tdl(ts, "EAED", "ADE", 2);
    tdl(ts, "AED", "EADE", 2);
    tdl(ts, "ab12", "de21", 3);
    tdl(ts, "bced", "cbde", 2);
    tdl(ts, "hello", "helllo", 1);
    tdl(ts, "vihik", "wihhil", 3);
    tdl(ts, "sahtel", "saddle", 3);
    tdl(ts, "abcbdeefg", "acbdbeefg", 2);
    tdl(ts, "LO WORL", "RO WOR", 2);
    tdl(ts, "AECDAED", "ECDEADE", 3);
    tdl(ts, "AECDEAED", "ECDADE", 3);
    tdl(ts, "telefon", "tekefon", 1);
    tdl(ts, "telefon", "tellefon", 1);
    tdl(ts, "EBEHMOT", "BEGEMOTH", 4);
    tdl(ts, "AAAA BBBB", "AAA BBBB", 1);
    tdl(ts, "I WANT BEER", "DEAR", 9);
    tdl(ts, "DEAR WORLD", "HELLO WORL", 5);
    tdl(ts, "HELLO WORLD", "HELO WORLD", 1);
    tdl(ts, "HELLO WORLD", "HELRO WORD", 2);
    tdl(ts, "HELLO WORLD", "HELLO DEAR WORLD", 5);
    tdl(ts, "what d o um ean", "wjat do u mean", 3);
    tdl(ts, "010", "10", 1);
    tdl(ts, "010", "101010", 3);
    tdl(ts, "010101010", "101010", 3);
    tdl(ts, "01010101010010101010", "101001011010010", 5);
    tdl(ts, "what d o um ean", "wjhat do you mean jb", 8);

    tla(ts, {0, 1, 2, 3, 4, 5, 6, 7, 8}, 12);
    tla(ts, {0, 0, 1, 4, 2, 3, 0, 4, 0}, 2);
    tla(ts, {1, 7, 8, 3, 7, 3, 5, 4, 5}, 8);
    tla(ts, {1, 1, 3, 2, 1, 4, 1, 2, 3}, 5);
    tla(ts, {1, 2, 1, 2, 1, 2, 3, 2, 1}, 3);
    tla(ts, {1, 2, 4, 2, 1, 6, 3, 4, 2}, 4);

    tla(ts, {
        5, 4, 3, 3,
        3, 2, 1, 2,
        4, 8, 6, 7,
        5, 5, 1, 3
    }, 10);

    tla(ts, {
        7, 8, 9, 3, 0,
        1, 1, 0, 4, 2,
        0, 5, 0, 5, 0,
        2, 2, 1, 0, 5,
        7, 3, 0, 4, 9
    }, 1);

    tla(ts, {5, 6, 9, 8, 7, 8,
             8, 4, 7, 3, 6, 7,
             3, 3, 1, 8, 2, 6,
             2, 7, 3, 4, 6, 3,
             3, 3, 2, 5, 9, 7,
             3, 7, 8, 9, 0, 8}, 15);
}

void test_random(Util::TestSuite& ts, Util::Random& r) {
    unsigned n = r.rand(1u, 10000u);
    std::string s = Util::StringUtil::repeat(" ", n);
    for (unsigned i = 0; i != n; ++i) {
        s[i] = (char) r.rand(32, 126);
    }
    std::string s2 = s;
    unsigned count = 0;
    std::string literal;
    for (unsigned i = r.rand(0u, 10u); i < n - 1; i += r.rand(2u, 10u)) {
        ++count;
        s2[i] = '~';
    }
    tl(ts, s, s2, count);
    tdl(ts, s, s2, count);
}

void test_stress() {
    Util::TestSuite ts("Stress Test", "\tPassed all");
    Util::Random r;

    unsigned test_count = 50;
    std::cout << "Stress-testing (" << test_count << " tests)..." << std::endl;

    Util::LoadingBar bar(test_count);
    for (unsigned i = 0; i != test_count; ++i) {
        bar.conditional_display(i, "\t", "\n");
        test_random(ts, r);
    }
    bar.conditional_display(test_count, "\t", "\n");
}

void test() {
    test_simple_cases();
    test_edge_cases();
    test_complex_cases();
    test_stress();
}

// Entry Point

int main() {
    test();
}
