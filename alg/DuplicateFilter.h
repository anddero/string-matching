//
// Created by karlandero on 22.01.21.
//

#ifndef FINDSONGDUPLICATES_DUPLICATEFILTER_H
#define FINDSONGDUPLICATES_DUPLICATEFILTER_H


#include <vector>
#include <map>
#include <list>
#include "SearchQueryDifference.h"

using String = std::string;

template <typename T, typename U>
using Pair = std::pair<T, U>;

template <typename T>
using List = std::list<T>;

template <typename T>
using Vector = std::vector<T>;

template <typename T, typename U>
using Map = std::map<T, U>;

struct SourceLine {
    std::string line;
    bool deleted;
    bool from_unique_source;

    SourceLine(const std::string &line, bool deleted, bool from_unique_source) : line(line), deleted(deleted), from_unique_source(from_unique_source) {}
};

struct DuplicateDetails {
    unsigned source_index;
    float similarity;
    std::vector<int> matching_original_line_word_indices;
    DuplicateDetails(unsigned int index, float similarity,
                     const std::vector<int> &matchingOriginalLineWordIndices)
            : source_index(index)
            , similarity(similarity)
            , matching_original_line_word_indices(matchingOriginalLineWordIndices) {}
};

using IndexDupMap = Map<unsigned, List<DuplicateDetails> >;

using IndexDupMapEl = Pair<unsigned, List<DuplicateDetails> >;

using DiffDupMap = Map<float, IndexDupMap>;

using DiffDupMapEl = Pair<float, IndexDupMap>;


class DuplicateFilter {
    Vector<SourceLine> source_lines;
    unsigned ref_line_index = 0;

    List<unsigned> skipped_unicode_lines;
    List<unsigned> unique_ascii_lines;
    DiffDupMap ascii_duplicates;

    bool scan_next();
    void write_dup_file(const String& file_name, IndexDupMap& index_dup_map);
    void write_unicode_file(const String& file_name) const;
    void write_unique_file(const String& file_name) const;
    unsigned move_remaining_sources();

    static bool difference_in_number(const SearchQuerySimilarityResult &similarity_result, const std::string &line, const std::string &ref_line);

public:
    /**
     * Construct the method-object, setting initial state, without any heavy processing.
     * @param source All the lines which will be processed for duplicates. The ordering of the lines may carry
     * some significance under certain conditions, see the method 'scan' for more information.
     * @param diff_upper_bounds The difference percentages to consider when scanning for duplicates. For example, the
     * list [0, 3, 5] would cause all the duplicates with similarity of 95% or higher to be grouped into 3 categories:
     * similarity of exactly 100%, similarity in range [97%, 100%) and similarity in range [95%, 97%). See 'finalize'
     * for more information.
     */
    DuplicateFilter(const std::vector<String> &source, const Vector<float>& diff_upper_bounds);
    unsigned get_source_count() const;
    unsigned get_skipped_count() const;
    unsigned get_unique_count() const;
    unsigned get_dup_count() const;
    unsigned get_next_index() const;
    unsigned get_deleted_count() const;
    /**
     * Scan the next 'count' lines. If this is the first call after object initialization, then the scanning will
     * process the lines with indices [0, count). If this is not the first call, then scanning will continue from the
     * line after the last one which was processed with the previous call. For example, if the previous call processed
     * the lines with indices [0, 15], then this call with count=5 will process the lines with indices [16, 20]. The
     * lines are taken from the given source on construction and will always be carried out in the same order as the
     * ordering in the given list.
     *
     * This means that ordering may be relevant. For example, if you have 100 lines which you know that are already
     * unique (or any other reason) and you want to exclude them from scanning, and you have 20 new lines which you
     * want to have processed, then you should place the 20 new lines in the beginning and the 100 excluded lines in
     * the end of the source list. You may then pass count=20 to this method and finish early, not wasting processing
     * power on the 100 lines which were already unique anyway and would not change the result.
     *
     * What does it mean to 'process a line'? For example, if the current line being processed has index 5 and there are
     * a total of 20 lines in the source, then all the lines with indices higher than 5 will be compared to the line
     * with index 5. In this case, line 5 will be compared with lines [6, 20]. If line 7 is considered a duplicate of
     * any of the given difference percentages, then it will be marked as a duplicate of line 5 with the highest
     * percentage that is lower or equal to the actual difference percentage. This means that line 7 will be skipped
     * in all the future comparisons and processing. Multiple lines could be marked as duplicates of line 5, if there
     * are others in addition to line 7 in this example.
     * @param count The number of lines to process next. Note that the average number of operations performed is much
     * higher. All the next 'count' lines will be compared to all the other lines following them in the list, therefore
     * the complexity will be on average (count * total_line_count * comparison operation complexity).
     * @return True if exactly 'count' lines were processed. False if less than 'count' lines were processed due to
     * reaching the end of list.
     */
    bool scan(unsigned count);
    /**
     * Output the results into files. For each percentage category, a separate file is created. Each one of these files
     * will contain the processed lines which have at least one duplicate. All the duplicates of each processed line
     * is output underneath the corresponding processed line as a list ordered from most similar to most different.
     * The percentage and the intermediate comparison form are also output per duplicate which better describes the
     * similarity score. Note that each one of the processed lines, whether it had duplicates or not, will also be
     * present in the file with the unique lines. There is also a separate file with lines which were ignored due to
     * containing non-ASCII values, these lines were completely excluded from all processing and comparison operations.
     * @param file_name_prefix The custom prefix to prepend to each output file. For example, the prefix 'out' will
     * cause all the generated file names to take the form 'out_<original-file-name>_<description>'.
     */
    void finalize(const String &file_name_prefix);

    static bool is_digit(const char &c);
};


#endif //FINDSONGDUPLICATES_DUPLICATEFILTER_H
