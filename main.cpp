#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include "alg/Operation.h"
#include "alg/WordDiffLite.h"

#define NO_DESCRIPTION "<no description>"

using namespace std;

// Utils

namespace Util {

    class Random {

        random_device *rd;
        mt19937 *mt;
        uniform_real_distribution<long double> *dist;

    public:

        Random(): rd{new random_device()},
                  mt{new mt19937((*rd)())},
                  dist{new uniform_real_distribution<long double>(0.0l, 1.0l)} {
        }

        ~Random() {
            delete dist;
            delete mt;
            delete rd;
        }

        template<typename NumericType>
        NumericType rand(NumericType a, NumericType b) { // TODO Test, also test negative ranges
            auto range = b - a;
            long double d = (*dist)(*mt);
            auto add = (NumericType) (d * range);
            NumericType res = a + add;
            if (res < a) return a;
            if (res > b) return b;
            return res;
        }

        bool rand_bool() { // TODO Test
            return (*dist)(*mt) < 0.5;
        }

    };

    string repeat(const string &s, int count) { // TODO Test
        if (count <= 0) return "";
        if (count == 1) return s;
        if (count == 2) return s + s;
        if (count == 3) return s + s + s;
        string half = repeat(s, count / 2);
        string even_part = half + half;
        if (count % 2) {
            return even_part + s;
        }
        return even_part;
    }

    class LoadingBar {

        static const int total_bar_length = 20;

        const int total;

        int prev_bar_length = 0;

    public:

        explicit LoadingBar(int total) : total{total} {}

        void conditional_display(int index, const string &prefix = "\t", const string &postfix = "\n") {
            float perc = total <= 0 ? 1.f : index / (float) total;

            auto bar_length = static_cast<int>(total_bar_length * perc);
            if (bar_length > prev_bar_length) {
                prev_bar_length = bar_length;
                int space_count = total_bar_length - bar_length;
                cout << prefix
                     << "[" << repeat("=", bar_length) << repeat(" ", space_count) << "] "
                     << to_string((int) (perc * 100)) << "%"
                     << postfix;
            }
        }

    };

    class TestSuite {

        const string name;
        const string fin_text;

        int pass_count = 0;

    public:

        explicit TestSuite(string name, string fin_text = "passed all"):
                name{std::move(name)},
                fin_text{std::move(fin_text)} {
            cout << "Starting test suite: " << this->name << "...";
        };

        int get_pass_count() const {
            return pass_count;
        }

        void check(const bool pass, const string &description = NO_DESCRIPTION) {
            if (!pass) {
                write_prefix(description);
                cerr << endl;
                exit(1);
            }
            ++pass_count;
        }

        template<typename T>
        void check(const T actual, const T expected, const string &description = NO_DESCRIPTION) {
            if (expected == actual) {
                ++pass_count;
                return;
            }
            write_prefix(description);
            cerr << "\n\t" << "Expected: " << expected
                 << "\n\t" << "Actual: " << actual
                 << "\n";
            exit(1);
        }

        template<typename T>
        void check(const T actual, const T expected, const T expected_diff, const string &description = NO_DESCRIPTION) {
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
                cerr << "\n\t" << "Expected: " << expected
                     << "\n\t" << "Actual: " << actual
                     << "\n\t" << "Diff: " << actual_diff << " > " << expected_diff
                     << "\n";
                exit(1);
            }
            ++pass_count;
        }

        template<typename T>
        void check(const vector<T> &actual, const vector<T> &expected, const string &description = NO_DESCRIPTION) {
            if (actual.size() != expected.size()) {
                write_prefix(description);
                cerr << "\n\t" << "Actual array has size " << actual.size() << ", but expected " << expected.size() << "\n";
                exit(1);
            }
            auto size = actual.size();
            for (auto i = 0ull; i != size; ++i) {
                if (actual[i] != expected[i]) {
                    auto min_i = i < 8 ? 0ull : i - 7;
                    auto max_i = size < 9 || i > size - 9 ? size : i + 8;
                    write_prefix(description);
                    cerr << "\n\t" << "Arrays differ at element " << i << ":"
                         << "\n\t" << "Displaying elements in range [" << min_i << "; " << max_i << ")"
                         << "\n\t" << "Expected: " << expected[i]
                         << "\n\t" << "Actual: " << actual[i]
                         << "\n\t" << "Expected array: [";
                    for (i = min_i; i != max_i; ++i) {
                        cerr << expected[i] << ((i == max_i - 1) ? "]" : " ");
                    }
                    cerr << "\n\t" << "Actual array: [";
                    for (i = min_i; i != max_i; ++i) {
                        cerr << actual[i] << ((i == max_i - 1) ? "]" : " ");
                    }
                    cerr << "\n";
                    exit(1);
                }
            }
            ++pass_count;
        }

        ~TestSuite() {
            cout << fin_text << " " << pass_count << endl;
        }

    private:

        void write_prefix(const string &description) {
            cerr << "Failed: " << (pass_count + 1) << ": " << description;
        }

    };

}

// Data Structures

// Algorithm

/*vector<Move> alg(const std::string &word_one, const std::string &word_two) {
    return get_diff(word_one, word_two);
}*/

Difference alg(const std::string &word_one, const std::string &word_two) {
    return calculate_diff(word_one, word_two);
}

// Testing

/*static const unsigned long long DEL_SCORE = 12;
static const unsigned long long ADD_SCORE = 13;
static const unsigned long long REPLACE_SCORE = 14;
static const unsigned long long SWAP_SCORE = 11;*/

/*void tr(Util::TestSuite& ts, std::string original, const std::string &what, const std::string &with, const std::string &expected_result) {
    replace(original, what, with);
    ts.check(original, expected_result);
}*/

/*void t(Util::TestSuite& ts, const std::string &word1, const std::string &word2, std::string expected_moves_str) {
    vector<Move> expected_moves;
    unsigned long long i1 = 0;
    unsigned long long i2 = 0;
    unsigned long long score = 0;
    for (char c : expected_moves_str) {
        switch (c) {
            case 'd':
                score += DEL_SCORE;
                expected_moves.emplace_back(score, Move::Remove, false, false, i1, i2);
                ++i1;
                break;
            case 'i':
                score += ADD_SCORE;
                expected_moves.emplace_back(score, Move::Insert, false, false, i1, i2);
                ++i2;
                break;
            case '-':
                expected_moves.emplace_back(score, Move::Skip, false, false, i1, i2);
                ++i1;
                ++i2;
                break;
            case 'r':
                score += DEL_SCORE;
                expected_moves.emplace_back(score, Move::Remove, true, false, i1, i2);
                ++i1;
                score += REPLACE_SCORE - DEL_SCORE;
                expected_moves.emplace_back(score, Move::Insert, false, true, i1, i2);
                ++i2;
                break;
            case 's':
                score += DEL_SCORE;
                expected_moves.emplace_back(score, Move::Remove, true, false, i1, i2);
                ++i1;
                expected_moves.emplace_back(score, Move::Skip, true, false, i1, i2);
                ++i1;
                ++i2;
                score += SWAP_SCORE - DEL_SCORE;
                expected_moves.emplace_back(score, Move::Insert, false, true, i1, i2);
                ++i2;
                break;
            default:
                throw logic_error("Unhandled move letter: " + c);
        }
    }
    ts.check(alg(word1, word2), expected_moves);
}*/

void t(Util::TestSuite& ts, const string &word1, const string &word2, const string &ops) {
    unsigned long long match_count = 0;
    unsigned long long change_count = 0;
    long long expansion_size = 0;
    float ratio;
    vector<Operation> operations;

    unsigned long long i1 = 0;
    unsigned long long i2 = 0;
    unsigned long long length = 0;
    Operation::Type op_type;
    for (char c : ops) {
        Operation::Type new_op_type;
        bool skip = false;
        switch (c) {
            case 'd':
                ++change_count;
                ++expansion_size;
                new_op_type = Operation::DELETE;
                break;
            case 'i':
                ++change_count;
                ++expansion_size;
                new_op_type = Operation::INSERT;
                break;
            case '-':
                ++match_count;
                skip = true;
                break;
            case 'r':
                ++change_count;
                new_op_type = Operation::REPLACE;
                break;
            default:
                throw logic_error("Unhandled operation letter: " + c);
        }
        if (skip || (new_op_type != op_type && length > 0)) {
            if (length > 0) {
                operations.emplace_back(op_type, i1, i2, length);
                switch (op_type) {
                    case Operation::DELETE:
                        i1 += length;
                        break;
                    case Operation::INSERT:
                        i2 += length;
                        break;
                    case Operation::REPLACE:
                        i1 += length;
                        i2 += length;
                        break;
                    default:
                        throw logic_error("Unhandled operation letter: " + c);
                }
            }
            if (skip) {
                ++i1;
                ++i2;
                length = 0;
            } else {
                length = 1;
            }
        } else {
            ++length;
        }
        op_type = new_op_type;
    }
    if (length > 0) {
        operations.emplace_back(op_type, i1, i2, length);
    }
    ratio = change_count / (float) std::max(1ull, std::max(word1.length(), word2.length()));
    Difference difference(0, 0, 0, 0);
    difference.match_count = match_count;
    difference.change_count = change_count;
    difference.expansion_size = expansion_size;
    difference.ratio = ratio;
    difference.operations = operations;

    ts.check(alg(word1, word2), difference);
}

void test_simple_cases() {
    Util::TestSuite ts("Simple Cases");

    /*tr(ts, "e", "e", "i", "i");
    tr(ts, "e", "f", "i", "e");
    tr(ts, "ee", "e", "i", "ii");
    tr(ts, "asd", "s", "e", "aed");
    tr(ts, "Hello world!", "world", "universe", "Hello universe!");*/

    /*t(ts, "", "a", "i");
    t(ts, "a", "a", "-");
    t(ts, "b", "a", "r");
    t(ts, "baa", "aba", "s-");
    t(ts, "ae", "ea", "s");
    t(ts, "ee", "ae", "r-");*/

    t(ts, "", "a", "i");
    t(ts, "a", "a", "-");
    t(ts, "b", "a", "r");
    t(ts, "ae", "ea", "d-i");
    t(ts, "ee", "ae", "r-");
    t(ts, "baa", "aba", "i--d");
}

void test_edge_cases() {
    Util::TestSuite ts("Edge Cases");

    /*tr(ts, "", "", "", "");
    tr(ts, "", "e", "r", "");
    tr(ts, "fefefe", "fe", "", "");
    tr(ts, "opk", "", "s", "sospsks");*/

    /*t(ts, "", "", "");
    t(ts, "", "google", "iiiiii");*/

    t(ts, "", "", "");
    t(ts, "", "google", "iiiiii");
}

void test_complex_cases() {
    Util::TestSuite ts("Complex Cases");

    /*tr(ts, "Hello world!", "l", "k", "Hekko workd!");
    tr(ts, "rr-s-d--r---i-rs-", "r", "di", "didi-s-d--di---i-dis-");
    tr(ts, "r-s-d-d-s-r-i-issrrrrisiirsrisirirsiirisrsir", "s", "d-i", "r-d-i-d-d-d-i-r-i-id-id-irrrrid-iiird-irid-iirird-iiirid-ird-iir");
    tr(ts, "r-d-i-d-d-d-i-r-i-id-id-irrrrid-iiird-irid-iirird-iiirid-ird-iir", "r", "di", "di-d-i-d-d-d-i-di-i-id-id-ididididiid-iiidid-idiid-iidiidid-iiidiid-idid-iidi");
*/
    /*t(ts, "abc", "def", "rrr");
    t(ts, "abc", "bac", "s-");
    t(ts, "AED", "ADE", "-s");
//    t(ts, "bcbd", "cbdb", "ss"); // TODO Don't test this special case by exact pattern, but score and number of moves
    t(ts, "LIME", "LIEM", "--s");
    t(ts, "Hello", "hekko", "r-rr-");
    t(ts, "EAED", "ADE", "d-s");
    t(ts, "AED", "EADE", "s-i");
    t(ts, "ab12", "de21", "rrs");
    t(ts, "hello", "helllo", "----i-");
    t(ts, "vihik", "wihhil", "r--i-r");
    t(ts, "sahtel", "saddle", "--rrs");
//    t(ts, "abcbdeefg", "acbdbeefg", "-ss----"); // TODO Don't test this special case by exact pattern, but score and number of moves
    t(ts, "LO WORL", "RO WOR", "r-----d");
    t(ts, "AECDAED", "ECDEADE", "d---s-i");
    t(ts, "AECDEAED", "ECDADE", "d---d-s");
    t(ts, "telefon", "tekefon", "--r----");
    t(ts, "telefon", "tellefon", "---i----");
//    t(ts, "EBEHMOT", "BEGEMOTH", "si-d---i"); // TODO Don't test this special case by exact pattern, but score and number of moves
    t(ts, "AAAA BBBB", "AAA BBBB", "---d-----");
    t(ts, "I WANT BEER", "DEAR", "dddddddr-r-");
    t(ts, "DEAR WORLD", "HELLO WORL", "r-rri-----d");
    t(ts, "HELLO WORLD", "HELO WORLD", "---d-------");
    t(ts, "HELLO WORLD", "HELRO WORD", "---r-----d-");
    t(ts, "HELLO WORLD", "HELLO DEAR WORLD", "------iiiii-----");
    t(ts, "what d o um ean", "wjat do u mean", "-r----d---s---");
    t(ts, "010", "10", "d--");
    t(ts, "010", "101010", "i---ii");
    t(ts, "010101010", "101010", "d------dd");
    t(ts, "01010101010010101010", "101001011010010", "d----d----dd----d---"); // d----d----r---d---dd   ss----sds---dddd
    t(ts, "what d o um ean", "wjhat do you mean jb", "-i-----d--ii-s---iii");*/

    t(ts, "abc", "def", "rrr");
    t(ts, "abc", "bac", "d-i-");
    t(ts, "AED", "ADE", "-d-i");
    t(ts, "bcbd", "cbdb", "d---i"); // TODO Don't test this special case by exact pattern, but score and number of moves
    t(ts, "LIME", "LIEM", "--d-i");
    t(ts, "Hello", "hekko", "r-rr-");
    t(ts, "EAED", "ADE", "d-d-i");
    t(ts, "AED", "EADE", "d-i-i");
    t(ts, "ab12", "de21", "rrd-i");
    t(ts, "hello", "helllo", "----i-");
    t(ts, "vihik", "wihhil", "r--i-r");
    t(ts, "sahtel", "saddle", "--rrd-i");
    t(ts, "abcbdeefg", "acbdbeefg", "-d---i----"); // TODO Don't test this special case by exact pattern, but score and number of moves
    t(ts, "LO WORL", "RO WOR", "r-----d");
    t(ts, "AECDAED", "ECDEADE", "d---d-i-i");
    t(ts, "AECDEAED", "ECDADE", "d---d-d-i");
    t(ts, "telefon", "tekefon", "--r----");
    t(ts, "telefon", "tellefon", "---i----");
    t(ts, "EBEHMOT", "BEGEMOTH", "d--ri---i"); // TODO Don't test this special case by exact pattern, but score and number of moves
    t(ts, "AAAA BBBB", "AAA BBBB", "---d-----");
    t(ts, "I WANT BEER", "DEAR", "rrd-dddddd-");
    t(ts, "DEAR WORLD", "HELLO WORL", "r-rri-----d");
    t(ts, "HELLO WORLD", "HELO WORLD", "---d-------");
    t(ts, "HELLO WORLD", "HELRO WORD", "---r-----d-");
    t(ts, "HELLO WORLD", "HELLO DEAR WORLD", "------iiiii-----");
    t(ts, "what d o um ean", "wjat do u mean", "-r----d---d-i---");
    t(ts, "010", "10", "d--");
    t(ts, "010", "101010", "i---ii");
    t(ts, "010101010", "101010", "d------dd");
    t(ts, "01010101010010101010", "101001011010010", "d----d----dd----d---"); // d----d----r---d---dd   ss----sds---dddd
    t(ts, "what d o um ean", "wjhat do you mean jb", "-i-----d--ii-d-i---iii");

    // TODO Continue... Write score tests, then implement sentence diff by disregarding word ordering.
}

/*void test_random(Util::TestSuite& ts, Util::Random& r) {
    unsigned n = r.rand(1u, 20u); // TODO 10000
    std::string s = repeat(" ", n);
    for (unsigned i = 0; i != n; ++i) {
        s[i] = (char) r.rand(32, 126);
    }
    std::string s2 = s;
    unsigned prev_i = 0;
    std::string literal;
    for (unsigned i = r.rand(0u, 10u); i < n - 1; i += r.rand(2u, 10u)) {
        literal += repeat("-", i - prev_i);
        prev_i = i;
        if (r.rand_bool() && s2[i] != s2[i + 1] && (i == 0 || s2[i - 1] != s2[i + 1]) && (i + 1 == n - 1 || s2[i] != s2[i + 2])) {
            std::swap(s2[i], s2[i + 1]);
            literal += "s";
            prev_i += 2;
        } else {
            s2[i] = '~';
            literal += "r";
            prev_i += 1;
        }
    }
    literal += repeat("-", n - prev_i);
    std::cout << std::endl;
    std::cout << "DEBUG: Case " << (ts.get_pass_count() + 1) << std::endl;
    std::cout << "DEBUG: " << s << std::endl;
    std::cout << "DEBUG: " << s2 << std::endl;
    std::cout << "DEBUG: " << literal << std::endl;
    t(ts, s, s2, literal);
}*/

void test_random(Util::TestSuite& ts, Util::Random& r) {
    unsigned n = r.rand(1u, 100u); // TODO 10000
    std::string s = Util::repeat(" ", n);
    for (unsigned i = 0; i != n; ++i) {
        s[i] = (char) r.rand(32, 126);
    }
    std::string s2 = s;
    unsigned prev_i = 0;
    std::string literal;
    for (unsigned i = r.rand(0u, 10u); i < n - 1; i += r.rand(2u, 10u)) {
        literal += Util::repeat("-", i - prev_i);
        prev_i = i;
        s2[i] = '~';
        literal += "r";
        prev_i += 1;
    }
    literal += Util::repeat("-", n - prev_i);
    /*std::cout << std::endl;
    std::cout << "DEBUG: Case " << (ts.get_pass_count() + 1) << std::endl;
    std::cout << "DEBUG: " << s << std::endl;
    std::cout << "DEBUG: " << s2 << std::endl;
    std::cout << "DEBUG: " << literal << std::endl;*/
    t(ts, s, s2, literal);
}

void test_stress() {
    Util::TestSuite ts("Stress Test", "\tPassed all");
    Util::Random r;

    unsigned test_count = 10000;
    cout << "Stress-testing (" << test_count << " tests)..." << endl;

    Util::LoadingBar bar(test_count);
    for (unsigned i = 0; i != test_count; ++i) {
        bar.conditional_display(i, "\t", "\n");
        test_random(ts, r);
    }
    bar.conditional_display(test_count, "\t", "\n");
}

void execute_test_mode() {
    test_simple_cases();
    test_edge_cases();
    test_complex_cases();
    test_stress();
}

// I/O Mode

    // TODO Implement IO utilities if necessary

void execute_io_mode() {
    // Read and parse input
        // TODO Read and parse input

    // Run the algorithm
        // TODO Run the algorithm

    // Format and write output
    cout << "<not implemented>"; // TODO Write output
    cout << endl;
}

// Entry Point

int main() {
    const bool test_mode = true; // TODO false
    if (test_mode) {
        execute_test_mode();
    } else {
        execute_io_mode();
    }
}
