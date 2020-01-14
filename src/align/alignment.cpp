#include "align/alignment.hpp"

#include <iostream>
#include <algorithm>

/**
 * @brief  ETA parameter marks how long of extra segment on
 * each side of reference will be used for alignment
 *
 * Example:
 *  - read:            XXXXXX
 *  - reference:    EEEYYYYYYEEE
 * where E letter marks extra position due to ETA=3
 */
#define ETA 10

static bool compareByPos(const mapping::Pair &lhs,
                         const mapping::Pair &rhs) {
    return lhs.second < rhs.second;
}

void alignment::completeAlign(const FastaRecord& read,
                              const FastaRecord& reference,
                              const mapping::Band& band,
                              SmithWaterman::MutationsTupleVector& mutations) {

    if (band.empty()) {
        return;
    }

    auto first_pair = std::min_element(band.begin(), band.end(), compareByPos);
    const Seed& first_seed = first_pair->second;

    uint32_t start_read = first_seed.getStartReadPostion();
    int32_t start_ref = static_cast<int32_t>(
                            first_seed.getStartReferencePostion() -
                            start_read - ETA);

    int32_t ref_out_of_range = 0;
    if(start_ref < 0) {
        ref_out_of_range = start_ref;
        start_ref = 0;
    }

    uint32_t seq_len = static_cast<uint32_t>(
                           static_cast<int32_t>(read.getSequence().size()) +
                           2 * ETA + ref_out_of_range);

    std::string ref_seq_segment = reference.getSequence().substr(
                                      static_cast<unsigned>(start_ref),
                                      seq_len);

    SmithWaterman sw(read.getSequence(), ref_seq_segment, false);

    sw.reconstruct_path(static_cast<unsigned>(start_ref), mutations);
}

void alignment::partialAlign(const FastaRecord& read,
                             const FastaRecord& reference,
                             const mapping::Band& band,
                             SmithWaterman::MutationsTupleVector& mutations) {

    // ignore empty bands
    if (band.empty()) {
        return;
    }

    // find first and last element in band
    auto minmax_el = std::minmax_element(band.begin(), band.end(), compareByPos);
    const Seed& first_seed = minmax_el.first->second;
    const Seed& last_seed = minmax_el.second->second;

    // included area on read
    uint32_t read_start = first_seed.getStartReadPostion() + first_seed.getSizeRead();
    uint32_t read_end = last_seed.getStartReadPostion();

    // included area on reference
    uint32_t ref_start = first_seed.getStartReferencePostion() + first_seed.getSizeReference();
    uint32_t ref_end = last_seed.getStartReferencePostion();

    SmithWaterman sw(
        read.getSequence().substr(read_start, read_end - read_start),
        reference.getSequence().substr(ref_start, ref_end - ref_start),
        false       // don't use blosum table
    );

    sw.reconstruct_path(ref_start, mutations);
}
