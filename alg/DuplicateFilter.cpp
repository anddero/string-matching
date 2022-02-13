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
    if (next_index >= source_lines.size()) {
        return false;
    }

    SourceLine& ref_word = source_lines[next_index];

    if (ref_word.second) {
        ++next_index;
        return true;
    }

    if (!Util::StringUtil::is_ascii(ref_word.first)) {
        skipped_unicode_lines.push_back(next_index);
        ref_word.second = true;
        ++next_index;
        return true;
    }

    for (unsigned i = next_index + 1; i != source_lines.size(); ++i) {
        SourceLine& word = source_lines[i];

        if (word.second) {
            continue;
        }

        if (!Util::StringUtil::is_ascii(word.first)) {
            skipped_unicode_lines.push_back(i);
            word.second = true;
            continue;
        }

        float similarity = search_query_similarity(word.first, ref_word.first);
        for (auto& diff_dup_pair : ascii_duplicates) {
            if (similarity >= 1.f - diff_dup_pair.first) {
                diff_dup_pair.second // the index-duplicate-map
                        .insert(IndexDupMapEl(next_index, {})) // attempt to insert
                        .first // either new inserted pair or existing if already existed
                        ->second // the list
                        .emplace_back(i, similarity);
                word.second = true;
                break;
            }
        }
    }

    unique_ascii_lines.push_back(next_index);
    ref_word.second = true;
    ++next_index;
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

float max_similarity(const List<Duplicate>& duplicates) {
    float max_similarity = 0.f;
    for (const auto& dup : duplicates) {
        max_similarity = std::max(max_similarity, dup.second);
    }
    return max_similarity;
}

void DuplicateFilter::write_dup_file(const String &file_name, IndexDupMap &index_dup_map) {
    Vector<unsigned> index_dup_map_keys;
    for (auto &it : index_dup_map) {
        index_dup_map_keys.push_back(it.first);
        it.second.sort(
                [](const Duplicate& a, const Duplicate& b){
                    return a.second > b.second;
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
        const String& ref_word = source_lines[key].first;
        const List<Duplicate>& dups = index_dup_map.at(key);
        if (dups.size() == 1) {
            out_file << (dup_no++) << "." << std::endl;
        } else {
            out_file << dup_no << ". - " << (dup_no += dups.size()) - 1 << "." << std::endl;
        }
        out_file << "[unique] " << ref_word << std::endl;
        for (const Duplicate& dup : dups) {
            out_file << "[" << std::fixed << (dup.second * 100.f) << "%] "
                     << source_lines[dup.first].first << std::endl;
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
    return next_index;
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
