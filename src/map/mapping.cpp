#include "map/mapping.hpp"

#include <cstdint>
#include <iostream>
#include <algorithm>

#define EPSILON 2


/**
 * @brief Get the bend lenght (ignoring duplicates)
 *
 * @tparam Iterator type of iterator passed to function
 * @param begin iterator object that points to first element in band
 * @param end iterator object that points to one past element in band
 * @return uint32_t
 */
template <typename Iterator>
static uint32_t get_band_lenght_duplicates(const Iterator& begin, const Iterator& end) {

    uint32_t len = begin->second.getSizeReference();
    for (auto it = begin + 1; it < end; ++it) {

        // ignore duplicates
        if (*it == *(it-1)) {
            continue;
        }

        len += it->second.getSizeReference();
    }
    return len;

}

/**
 * @brief Get the bend lenght
 *
 *
 * @tparam Iterator type of iterator passed to function
 * @param begin iterator object that points to first element in band
 * @param end iterator object that points to one past element in band
 * @return uint32_t
 */
template <typename Iterator>
static uint32_t get_band_lenght(const Iterator& begin, const Iterator& end) {

    uint32_t len = begin->second.getSizeReference();
    for (auto it = begin + 1; it < end; ++it) {

        len += it->second.getSizeReference();
    }
    return len;

}

mapping::Band mapping::minexmap(FastaRecord& read,
                                FastaRecord& reference,
                                FastaRecord::MinimizersTable& reference_minimizers,
                                short w,
                                short k) {

    std::vector<mapping::Pair> hits;

    FastaRecord::MinimizersTable read_minimizers = read.getMinimizers(k, w);

    // collects minimizer hits between read and reference
    for (const auto& read_minimizer_pos : read_minimizers) {

        // find hits (exact matches)
        std::string read_minimizer = read_minimizer_pos.first; // minimizer value
        auto reference_minimizer_pos = reference_minimizers.find(read_minimizer);
        if (reference_minimizer_pos == reference_minimizers.end()) {
            continue;
        }

        // for all matched positions append differences and seeds to hits
        std::unordered_set<uint32_t> read_positions = read_minimizer_pos.second;
        std::unordered_set<uint32_t> reference_positions = reference_minimizer_pos->second;

        for (const uint32_t& read_pos: read_positions) {
            for (const uint32_t& reference_pos : reference_positions) {

                // make seed from hit and extend it to both directions
                Seed seed(read_pos, reference_pos, read_minimizer.size());
                seed.extendBoth(read.getSequence(), reference.getSequence());

                int32_t diff = static_cast<int32_t>(seed.getStartReadPostion() - seed.getStartReferencePostion());
                hits.push_back(std::make_pair(diff, seed));
            }
        }
    }

    // sorts hits in order of difference, reference_position
    std::sort(hits.begin(), hits.end());


    uint32_t max_band_lenght = 0;
    std::vector<mapping::Pair>::iterator max_set_begin;
    std::vector<mapping::Pair>::iterator max_set_end;

    // preforms single linkage clustering to group approximately colinear hits
    auto b = hits.begin();
    for(auto e = hits.begin(); e < hits.end(); ++e) {

        if (e+1 == hits.end() || (e+1)->first - e->first >= EPSILON ) {

            // find longest approximately colinear set of hits
            uint32_t band_lenght = get_band_lenght_duplicates(b, e+1);
            if (band_lenght > max_band_lenght) {
                max_band_lenght = band_lenght;
                max_set_begin = b;
                max_set_end = e+1;
            }

            b = e + 1;
        }
    }

    return mapping::Band(max_set_begin, max_set_end);
}

mapping::Band mapping::minimap(FastaRecord& read,
                               FastaRecord::MinimizersTable& reference_minimizers,
                               short w,
                               short k) {

    std::vector<mapping::Pair> hits;

    FastaRecord::MinimizersTable read_minimizers = read.getMinimizers(k, w);
    for (const auto& read_minimizer_pos : read_minimizers) {

        // find exact matches
        std::string read_minimizer = read_minimizer_pos.first; // minimizer value
        auto reference_minimizer_pos = reference_minimizers.find(read_minimizer);
        if (reference_minimizer_pos == reference_minimizers.end()) {
            continue;
        }

        // for all matched positions append differences to hits
        std::unordered_set<uint32_t> read_positions = read_minimizer_pos.second;
        std::unordered_set<uint32_t> reference_positions = reference_minimizer_pos->second;

        for (const uint32_t& read_pos: read_positions) {
            for (const uint32_t& reference_pos : reference_positions) {

                // make seed from hit
                Seed seed(read_pos, reference_pos, read_minimizer.size());

                int32_t diff = static_cast<int32_t>(seed.getStartReadPostion() - seed.getStartReferencePostion());
                hits.push_back(std::make_pair(diff, seed));
            }
        }
    }

    // sorts hits in order of difference, reference_position
    std::sort(hits.begin(), hits.end());


    uint32_t max_band_lenght = 0;
    std::vector<mapping::Pair>::iterator max_set_begin;
    std::vector<mapping::Pair>::iterator max_set_end;

    // preforms single linkage clustering to group approximately colinear hits
    auto b = hits.begin();
    for(auto e = hits.begin(); e < hits.end(); ++e) {

        if (e+1 == hits.end() || (e+1)->first - e->first >= EPSILON ) {

            // find longest approximately colinear set of hits
            uint32_t band_lenght = get_band_lenght(b, e+1);
            if (band_lenght > max_band_lenght) {
                max_band_lenght = band_lenght;
                max_set_begin = b;
                max_set_end = e+1;
            }

            b = e + 1;
        }
    }

    return mapping::Band(max_set_begin, max_set_end);
}
