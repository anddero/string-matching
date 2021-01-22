#include <string>
#include <iostream>
#include <algorithm>
#include "util/TestSuite.h"
#include "alg/Levenshtein.h"
#include "alg/DamerauLevenshtein.h"
#include "util/Random.h"
#include "util/StringUtil.h"
#include "util/LoadingBar.h"
#include "alg/MinimumLinearAssignment.h"
#include "alg/SearchQueryDifference.h"
#include "alg/DuplicateFilter.h"

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

void tsq(Util::TestSuite& ts, const std::string &query1, const std::string &query2, const unsigned expected_score) {
    ts.check(search_query_difference(query1, query2), expected_score);
}

void tsim(Util::TestSuite& ts, const std::string &query1, const std::string &query2, const unsigned expected_percentage) {
    ts.check(search_query_similarity(query1, query2), expected_percentage / 100.f, 0.01f);
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

    tsq(ts, "", "say my name", 9);
    tsq(ts, "i i", "i e", 1);
    tsq(ts, "hello", "helko", 1);
    tsq(ts, "lamp", "palm", 3);
    tsq(ts, "ramp", "dms", 3);
    tsq(ts, "salat", "padi", 4);
    tsq(ts, "lememe", "leememem", 2);
    tsq(ts, "porgand", "pokker", 5);
    tsq(ts, "redmine", "remedine", 3);
    tsq(ts, "calculate", "relate", 5);

    tsim(ts, "", "say my name", 0);
    tsim(ts, "i i", "i e", 50);
    tsim(ts, "hello", "helko", 80);
    tsim(ts, "lamp", "palm", 25);
    tsim(ts, "ramp", "dms", 25);
    tsim(ts, "salat", "padi", 20);
    tsim(ts, "lememe", "leememem", 75);
    tsim(ts, "porgand", "pokker", 29);
    tsim(ts, "redmine", "remedine", 62);
    tsim(ts, "calculate", "relate", 44);
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

    tsq(ts, "", "", 0);
    tsq(ts, "a", " a", 0);
    tsq(ts, "   ", "  ", 0);
    tsq(ts, "  --_? ", " \"a''  ", 1);
    tsq(ts, "ke$ha==name", "name,kesha", 1);

    tsim(ts, "", "", 100);
    tsim(ts, "a", " a", 100);
    tsim(ts, "   ", "  ", 100);
    tsim(ts, "  --_? ", " \"a''  ", 0);
    tsim(ts, "ke$ha==name", "name,kesha", 89);
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

    tsq(ts, "justin jutsin", "ustin justni", 3);
    tsq(ts, "say my name", "my naem say", 1);
    tsq(ts, "tom jones", "joonas", 5);
    tsq(ts, "cascada", "carmen kass", 9);
    tsq(ts, "pavarotti", "parlov ott", 9);
    tsq(ts, "k2bilauler", "mobile ring", 4 + 6);
    tsq(ts, "penetrate", "penelope terrate", 4 + 7);
    tsq(ts, "justin bieber", "justin tiembrlake", 7);
    tsq(ts, "best song ever", "westlife you never", 1 + 3 + 5);
    tsq(ts, "artist name - song name", "name - artist name", 4);
    tsq(ts, "evanescense everytime i", "evreytiem evanscence", 5);
    tsq(ts, "flo rida whistle", "Flo Rida Whistle", 0);
    tsq(ts, "flo roda whistle", "whistle flo rida", 1);
    tsq(ts, "outside calvin harris ft ellie goulding", "ELLIE CALVIN HARRIS OUTSIDE", 10);
    tsq(ts, "ellie goulding goodness gracious", "good ness gracious ellie", 9);
    tsq(ts, "need your love ft. calvin harris ellie goulding",
            "Calvin Harris ft Ellie Goulding - I Need Your Love", 1);
    tsq(ts, "oustide calvin harris ft. ellie goulding",
            "outside - calvin harris ft ellie goulding", 1);
    tsq(ts, "sOMe VeRY gGOOD Song", "SOme very OGOD song", 2);
    tsq(ts, "i love this song", "I LOVE SONG THIS", 0);

    tsim(ts, "justin jutsin", "ustin justni", 75);
    tsim(ts, "say my name", "my naem say", 89);
    tsim(ts, "tom jones", "joonas", 37);
    tsim(ts, "cascada", "carmen kass", 10);
    tsim(ts, "pavarotti", "parlov ott", 0);
    tsim(ts, "k2bilauler", "mobile ring", 0);
    tsim(ts, "penetrate", "penelope terrate", 27);
    tsim(ts, "justin bieber", "justin tiembrlake", 56);
    tsim(ts, "best song ever", "westlife you never", 44);
    tsim(ts, "artist name - song name", "name - artist name", 78);
    tsim(ts, "evanescense everytime i", "evreytiem evanscence", 76);
    tsim(ts, "flo rida whistle", "Flo Rida Whistle", 100);
    tsim(ts, "flo roda whistle", "whistle flo rida", 93);
    tsim(ts, "outside calvin harris ft ellie goulding", "ELLIE CALVIN HARRIS OUTSIDE", 71);
    tsim(ts, "ellie goulding goodness gracious", "good ness gracious ellie", 69);
    tsim(ts, "need your love ft. calvin harris ellie goulding",
             "Calvin Harris ft Ellie Goulding - I Need Your Love", 97);
    tsim(ts, "oustide calvin harris ft. ellie goulding",
             "outside - calvin harris ft ellie goulding", 97);
    tsim(ts, "sOMe VeRY gGOOD Song", "SOme very OGOD song", 88);
    tsim(ts, "i love this song", "I LOVE SONG THIS", 100);
}

void test_random(Util::TestSuite& ts, Util::Random& r) {
    unsigned n = r.rand(1u, 1000u);
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

void test_random_sq(Util::TestSuite& ts, Util::Random& r) {
    unsigned word_count = r.rand(1u, 10u);
    std::string s;
    for (unsigned i = 0; i != word_count; ++i) {
        unsigned word_length = r.rand(1u, 10u);
        for (unsigned j = 0; j != word_length; ++j) {
            s.push_back((char) r.rand(32, 127));
        }
        s += " ";
    }
    tsq(ts, s, s, 0);
    std::string s2 = s;
    for (unsigned i = 0; i < s.size(); i+=2) {
        if (r.rand_bool()) {
            std::string cut = s2.substr(i, 2);
            cut = Util::StringUtil::lowercase(cut);
            s2.replace(i, 2, cut);
        }
    }
    tsq(ts, s, s2, 0);
}

void test_stress() {
    Util::TestSuite ts("Stress Test", "\tPassed all");
    Util::Random r;

    unsigned test_count = 100;
    unsigned test_count_sq = 100000;
    std::cout << "Stress-testing (" << test_count << " tests)..." << std::endl;

    Util::LoadingBar bar(test_count + test_count_sq);
    for (unsigned i = 0; i != test_count; ++i) {
        bar.conditional_display(i, "\t", "\n");
        test_random(ts, r);
    }
    for (unsigned i = 0; i != test_count_sq; ++i) {
        bar.conditional_display(i + test_count, "\t", "\n");
        test_random_sq(ts, r);
    }
    bar.conditional_display(test_count + test_count_sq, "\t", "\n");
}

void test() {
    test_simple_cases();
    test_edge_cases();
    test_complex_cases();
    test_stress();
}

// Entry Point

template<typename Numeric1, typename Numeric2>
float get_perc(Numeric1 amount, Numeric2 max) {
    return amount / (float) max * 100.f;
}

int main() {
    std::cout << "Relative path to file ('exit' to abort): ";
    std::string path; std::cin >> path;
    if (path == "exit") return 0;

    std::cout << "Prefix for newly created files ('exit' to abort): ";
    std::string prefix; std::cin >> prefix;
    if (prefix == "exit") return 0;

    std::cout << "Comma-separated difference integers to consider ('exit' to abort): ";
    std::string diff_list; std::cin >> diff_list;
    if (diff_list == "exit") return 0;

    std::vector<std::string> diffs_s = Util::StringUtil::split(diff_list, ",");
    std::vector<float> diffs(diffs_s.size());

    std::transform(
            diffs_s.begin(),
            diffs_s.end(),
            diffs.begin(),
            [](const std::string& diff_percentage) {
                return std::stoi(diff_percentage) / 100.f;
            }
    );

    DuplicateFilter duplicate_filter(
            path,
            diffs
    );

    unsigned int source_count = duplicate_filter.get_source_count();
    std::cout << std::endl << "Duplicate filter has been initialized with "
              << source_count << " source entries." << std::endl;

    if (source_count == 0) {
        std::cout << "Nothing to do with 0 entries, type anything and hit enter to exit." << std::endl;
        std::string exit_command; std::cin >> exit_command;
        return 0;
    }

    bool play = true;
    while (play) {
        std::cout << std::endl << "Insert the number of entries to scan next ('0' = finish): ";
        unsigned scan_count; std::cin >> scan_count;
        scan_count = std::min(scan_count, source_count - duplicate_filter.get_next_index() + 1);
        if (scan_count == 0) break;

        Util::LoadingBar loading_bar(scan_count);
        for (unsigned i = 0; i != scan_count; ++i) {
            play = duplicate_filter.scan(1);
            loading_bar.conditional_display(i);
            if (!play) break;
        }
        loading_bar.conditional_display(scan_count);

        std::cout.precision(2);
        const unsigned int scanned_count = duplicate_filter.get_next_index();
        const unsigned int deleted_count = duplicate_filter.get_deleted_count();
        const unsigned int duplicate_count = duplicate_filter.get_dup_count();
        const unsigned int unique_count = duplicate_filter.get_unique_count();
        const unsigned int skipped_count = duplicate_filter.get_skipped_count();
        std::cout << std::endl << "Progress (out of " << source_count << " total):" << std::endl
                  << "\tScanned: " << scanned_count
                        << " // " << std::fixed << get_perc(scanned_count, source_count) << "% of total" << std::endl
                  << "\tDeleted: " << deleted_count
                        << " // " << std::fixed << get_perc(deleted_count, source_count) << "% of total" << std::endl
                  << "\tDuplicates found: " << duplicate_count
                        << " // " << std::fixed << get_perc(duplicate_count, source_count) << "% of total" << std::endl
                  << "\tUnique elements recorded: " << unique_count
                        << " // " << std::fixed << get_perc(unique_count, scanned_count) << "% of scanned" << std::endl
                  << "\tNon-ASCII elements skipped: " << skipped_count
                        << " // " << std::fixed << get_perc(skipped_count, source_count) << "% of total" << std::endl;
    }

    duplicate_filter.finalize(prefix);
    std::cout << std::endl << "Files have been written to disk. Type anything and hit enter to close." << std::endl;
    std::string exit_command; std::cin >> exit_command;
}
