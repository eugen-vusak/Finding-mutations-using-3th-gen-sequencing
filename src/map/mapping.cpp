#include "map/mapping.hpp"

#include "map/Seed.hpp"

#include <cstdint>
#include <iostream>
#include <algorithm>

void Mapping::minexmap(FastaRecord& read,
                       FastaRecord& reference,
                       FastaRecord::MinimizersTable& reference_minimizers,
                       short w,
                       short k) {

    std::vector<std::pair<int32_t, Seed>> array;


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

                int32_t diff = static_cast<int32_t>(seed.start_pos_read_ - seed.start_pos_reference_);
                array.push_back(std::make_pair(diff, seed));
            }
        }
    }

    std::sort(array.begin(), array.end());
    // std::sort(array.begin(), array.end(), std::greater<std::array<int32_t, 2>>());

    for (const auto& item : array) {
        std::cout << item.first << ", " << item.second << std::endl;
    }


    // b = 1
    // for e in array.size():
    //     if A[e+1].c - A[e].c >= eps:
    //         C - max_col_subset(A[b..e])
    //         print
    //         b = e + 1
}
