//
// Created by karlandero on 22.01.21.
//

#ifndef FINDSONGDUPLICATES_DUPLICATEFILTER_H
#define FINDSONGDUPLICATES_DUPLICATEFILTER_H


#include <vector>
#include <map>
#include <list>

using String = std::string;

template <typename T, typename U>
using Pair = std::pair<T, U>;

template <typename T>
using List = std::list<T>;

template <typename T>
using Vector = std::vector<T>;

template <typename T, typename U>
using Map = std::map<T, U>;

using SourceLine = Pair<String, bool>;

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

public:
    DuplicateFilter(const std::vector<String> &source, const Vector<float>& diff_upper_bounds);
    unsigned get_source_count() const;
    unsigned get_skipped_count() const;
    unsigned get_unique_count() const;
    unsigned get_dup_count() const;
    unsigned get_next_index() const;
    unsigned get_deleted_count() const;
    bool scan(unsigned count);
    void finalize(const String &file_name_prefix);
};


#endif //FINDSONGDUPLICATES_DUPLICATEFILTER_H
