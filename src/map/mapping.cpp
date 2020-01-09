#include "map/mapping.hpp"

#include <cstdint>
#include <iostream>
#include <algorithm>

#define EPSILON 2

static uint32_t get_band_lenght_duplicates(mapping::Band band) {

    uint32_t len = band.begin()->second.getSize();

    for (auto it = band.begin() + 1; it < band.end(); ++it) {
        if (*it == *(it-1)) {
            continue;
        }

        len += it->second.getSize();
    }

    return len;
}

static uint32_t get_band_lenght(mapping::Band band) {
    uint32_t len = 0u;
    for (auto pair : band) {
        len += pair.second.getSize();
    }
    return len;
}

mapping::Band mapping::minexmap(FastaRecord& read,
                                FastaRecord& reference,
                                FastaRecord::MinimizersTable& reference_minimizers,
                                short w,
                                short k) {

    std::vector<mapping::Pair> array;

    FastaRecord::MinimizersTable read_minimizers = read.getMinimizers(k, w);
    for (const auto& read_minimizer_pos : read_minimizers) {

        // find exact matches
        std::string read_minimizer = read_minimizer_pos.first; // minimizer value
        auto reference_minimizer_pos = reference_minimizers.find(read_minimizer);
        if (reference_minimizer_pos == reference_minimizers.end()) {
            continue;
        }

        // for all matched positions append differences to array
        std::unordered_set<uint32_t> read_positions = read_minimizer_pos.second;
        std::unordered_set<uint32_t> reference_positions = reference_minimizer_pos->second;

        for (const uint32_t& read_pos: read_positions) {
            for (const uint32_t& reference_pos : reference_positions) {

                Seed seed(read_pos, reference_pos, read_minimizer.size());
                seed.extendBoth(read.getSequence(), reference.getSequence());

                int32_t diff = static_cast<int32_t>(seed.getStartReadPostion() - seed.getStartReferencePostion());
                array.push_back(std::make_pair(diff, seed));
            }
        }
    }

    std::sort(array.begin(), array.end());

    // find best band from all matches
    uint32_t max_band_lenght = 0;
    mapping::Band best_band;

    auto b = array.begin();
    for(auto e = array.begin(); e < array.end(); ++e) {

        if (e+1 == array.end() || (e+1)->first - e->first >= EPSILON ) {

            mapping::Band band(b, e+1);

            uint32_t band_lenght = get_band_lenght_duplicates(band);
            if (band_lenght > max_band_lenght) {
                max_band_lenght = band_lenght;
                best_band = band;
            }

            b = e + 1;
        }
    }

    return best_band;
}

mapping::Band mapping::minimap(FastaRecord& read,
                               FastaRecord::MinimizersTable& reference_minimizers,
                               short w,
                               short k) {

    std::vector<mapping::Pair> array;

    FastaRecord::MinimizersTable read_minimizers = read.getMinimizers(k, w);
    for (const auto& read_minimizer_pos : read_minimizers) {

        // find exact matches
        std::string read_minimizer = read_minimizer_pos.first; // minimizer value
        auto reference_minimizer_pos = reference_minimizers.find(read_minimizer);
        if (reference_minimizer_pos == reference_minimizers.end()) {
            continue;
        }

        // for all matched positions append differences to array
        std::unordered_set<uint32_t> read_positions = read_minimizer_pos.second;
        std::unordered_set<uint32_t> reference_positions = reference_minimizer_pos->second;

        for (const uint32_t& read_pos: read_positions) {
            for (const uint32_t& reference_pos : reference_positions) {

                Seed seed(read_pos, reference_pos, read_minimizer.size());

                int32_t diff = static_cast<int32_t>(seed.getStartReadPostion() - seed.getStartReferencePostion());
                array.push_back(std::make_pair(diff, seed));
            }
        }
    }

    std::sort(array.begin(), array.end());

    // find best band from all matches
    uint32_t max_band_lenght = 0;
    mapping::Band best_band;

    auto b = array.begin();
    for(auto e = array.begin(); e < array.end(); ++e) {

        if (e+1 == array.end() || (e+1)->first - e->first >= EPSILON ) {

            mapping::Band band(b, e+1);

            uint32_t band_lenght = get_band_lenght(band);
            if (band_lenght > max_band_lenght) {
                max_band_lenght = band_lenght;
                best_band = band;
            }

            b = e + 1;
        }
    }

    return best_band;
}
