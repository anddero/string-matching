//
// Created by karlandero on 22.01.21.
//

#include <fstream>
#include <algorithm>
#include "DuplicateFilter.h"
#include "../util/StringUtil.h"
#include "SearchQueryDifference.h"

DuplicateFilter::DuplicateFilter(const std::vector<String> &source, const Vector<float>& diff_upper_bounds) {
    for (const String& line : source) {
        source_lines.push_back(SourceLine(line, false));
    }

    for (float bound : diff_upper_bounds) {
        ascii_duplicates.insert(DiffDupMapEl(bound, {}));
    }
}

bool DuplicateFilter::scan_next() {
    if (ref_line_index >= source_lines.size()) {
        return false;
    }

    SourceLine& ref_line = source_lines[ref_line_index];

    if (ref_line.second) {
        ++ref_line_index;
        return true;
    }

    if (!Util::StringUtil::is_ascii(ref_line.first)) {
        skipped_unicode_lines.push_back(ref_line_index);
        ref_line.second = true;
        ++ref_line_index;
        return true;
    }

    for (unsigned line_index = ref_line_index + 1; line_index != source_lines.size(); ++line_index) {
        SourceLine& line = source_lines[line_index];

        if (line.second) {
            continue;
        }

        if (!Util::StringUtil::is_ascii(line.first)) {
            skipped_unicode_lines.push_back(line_index);
            line.second = true;
            continue;
        }

        const SearchQuerySimilarityResult similarity_result = search_query_similarity(line.first, ref_line.first);
        const auto& matching_ref_word_index_by_word_index = similarity_result.matching_query2_word_index_by_query1_word_index;
        float similarity = similarity_result.similarity;
        for (auto& diff_dup_pair : ascii_duplicates) {
            if (similarity >= 1.f - diff_dup_pair.first) {
                diff_dup_pair.second // the index-duplicate-map
                        .insert(IndexDupMapEl(ref_line_index, {})) // attempt to insert
                        .first // either new inserted pair or existing if already existed
                        ->second // the list
                        .emplace_back(line_index, similarity, matching_ref_word_index_by_word_index);
                line.second = true;
                break;
            }
        }
    }

    unique_ascii_lines.push_back(ref_line_index);
    ref_line.second = true;
    ++ref_line_index;
    return true;
}

bool DuplicateFilter::scan(unsigned count) {
    for (unsigned i = 0; i != count; ++i) {
        if (!scan_next()) {
            return false;
        }
    }
    return true;
}

float max_similarity(const List<DuplicateDetails>& duplicates) {
    float max_similarity = 0.f;
    for (const auto& dup : duplicates) {
        max_similarity = std::max(max_similarity, dup.similarity);
    }
    return max_similarity;
}

bool contains_negative_value(const std::vector<int>& v) {
    for (const auto &item : v) {
        if (item < 0) return true;
    }
    return false;
}

int max(const std::vector<int>& v) {
    if (v.empty()) throw std::logic_error("Should not pass empty list");
    int val = v[0];
    for (const auto &item : v) val = std::max(item, val);
    return val;
}

std::vector<int> filter_negative_values(const std::vector<int>& v) {
    std::vector<int> result;
    for (const auto &item : v) if (item >= 0) result.push_back(item);
    return result;
}

std::vector<int> remap_potentially_exceeding_values(const std::vector<int>& v) {
    const auto flipped = flip_matching_indices_expanding(v, max(v) + 1);
    const auto filtered = filter_negative_values(flipped);
    return flip_matching_indices(filtered);
}

std::string reorder_words(const std::string &phrase, std::vector<int> index_mapping) {
    const auto words = get_words(phrase);
    if (words.empty() || index_mapping.empty()) return phrase;

    if (!contains_negative_value(index_mapping)) {
        index_mapping = remap_potentially_exceeding_values(index_mapping);
    }

    unsigned next_unmapped_index = words.size() - 1;
    std::vector<std::string> result(words.size());
    for (unsigned i = 0; i != words.size(); ++i) {
        int new_index = index_mapping[i];
        if (new_index < 0 || new_index >= words.size()) {
            result[next_unmapped_index--] = words[i];
            continue;
        }
        result[new_index] = words[i];
    }
    return Util::StringUtil::join(result, " ");
}

void DuplicateFilter::write_dup_file(const String &file_name, IndexDupMap &index_dup_map) {
    Vector<unsigned> index_dup_map_keys;
    for (auto &it : index_dup_map) {
        index_dup_map_keys.push_back(it.first);
        it.second.sort(
                [](const DuplicateDetails& a, const DuplicateDetails& b){
                    return a.similarity > b.similarity;
                }
        );
    }
    std::sort(
            index_dup_map_keys.begin(),
            index_dup_map_keys.end(),
            [&index_dup_map](const unsigned a, const unsigned b){
                return max_similarity(index_dup_map.at(a)) > max_similarity(index_dup_map.at(b));
            }
    );

    std::ofstream out_file(file_name);
    out_file.precision(2);
    unsigned dup_no = 1;
    for (const unsigned key : index_dup_map_keys) {
        const String& ref_line = source_lines[key].first;
        const auto ref_line_normalized = Util::StringUtil::join(get_words(ref_line), " ");
        const List<DuplicateDetails>& duplicates = index_dup_map.at(key);
        std::vector<std::string> dup_lines_normalized;
        for (const auto &dup : duplicates) {
            dup_lines_normalized.push_back(reorder_words(source_lines[dup.source_index].first, dup.matching_original_line_word_indices));
        }
        if (duplicates.size() == 1) {
            out_file << (dup_no++) << "." << std::endl;
        } else {
            out_file << dup_no << ". - " << (dup_no += duplicates.size()) - 1 << "." << std::endl;
        }
        unsigned max_normalized_phrase_len = ref_line_normalized.length();
        for (const auto &item : dup_lines_normalized) {
            max_normalized_phrase_len = std::max<unsigned>(max_normalized_phrase_len, item.length());
        }
        out_file << "[unique] " << Util::StringUtil::pad_right(ref_line_normalized, ' ', max_normalized_phrase_len) << " ---|||--- " << ref_line << std::endl;
        unsigned ii = 0;
        for (const DuplicateDetails& dup : duplicates) {
            out_file << "[" << std::fixed << (dup.similarity * 100.f) << "%] "
                     << Util::StringUtil::pad_right(dup_lines_normalized[ii], ' ', max_normalized_phrase_len) << " ---|||--- "
                     << source_lines[dup.source_index].first << std::endl;
            ++ii;
        }
        out_file << std::endl;
    }
    out_file.close();
}

void DuplicateFilter::write_unicode_file(const String &file_name) const {
    std::ofstream out_file(file_name);
    for (const unsigned key : skipped_unicode_lines) {
        out_file << source_lines[key].first << std::endl;
    }
    out_file.close();
}

void DuplicateFilter::write_unique_file(const String &file_name) const {
    std::ofstream out_file(file_name);
    for (const unsigned key : unique_ascii_lines) {
        out_file << source_lines[key].first << std::endl;
    }
    out_file.close();
}

void DuplicateFilter::finalize(const String &file_name_prefix) {
    unsigned unscanned_count = move_remaining_sources();

    float max_diff = 0.f;
    for (const auto& diff_dup_pair : ascii_duplicates) {
        float diff = diff_dup_pair.first;
        max_diff = diff;
        IndexDupMap index_dup_pair = diff_dup_pair.second;
        write_dup_file(file_name_prefix + "_dup_" + std::to_string((1.f - diff) * 100.f) + ".txt", index_dup_pair);
    }

    write_unicode_file(file_name_prefix + "_unicode.txt");

    String unscanned_term = unscanned_count > 0 ? "_" + std::to_string(unscanned_count) + "unscanned" : "";

    write_unique_file(file_name_prefix + "_unique_" + std::to_string(max_diff * 100.f) + unscanned_term + ".txt");
}

unsigned DuplicateFilter::get_source_count() const {
    return (unsigned) source_lines.size();
}

unsigned DuplicateFilter::get_skipped_count() const {
    return (unsigned) skipped_unicode_lines.size();
}

unsigned DuplicateFilter::get_unique_count() const {
    return (unsigned) unique_ascii_lines.size();
}

unsigned DuplicateFilter::get_dup_count() const {
    unsigned count = 0;
    for (const auto& el : ascii_duplicates) {
        for (const auto& ell : el.second) {
            count += ell.second.size();
        }
    }
    return count;
}

unsigned DuplicateFilter::get_next_index() const {
    return ref_line_index;
}

unsigned DuplicateFilter::get_deleted_count() const {
    unsigned count = 0;
    for (const auto& el : source_lines) {
        count += el.second;
    }
    return count;
}

unsigned DuplicateFilter::move_remaining_sources() {
    unsigned count = 0;
    for (unsigned i = 0; i != source_lines.size(); ++i) {
        auto& el = source_lines[i];
        if (!el.second) {
            ++count;
            el.second = true;
            if (Util::StringUtil::is_ascii(el.first)) {
                unique_ascii_lines.push_back(i);
            } else {
                skipped_unicode_lines.push_back(i);
            }
        }
    }
    return count;
}
