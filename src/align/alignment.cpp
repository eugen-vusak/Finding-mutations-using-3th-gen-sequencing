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
#define SW_BAND_WIDTH 10

/**
 * @brief comparator for mapping::Pair type
 *
 * @param lhs pair one
 * @param rhs pair two
 * @return true if seed in pair one is smaller
 * @return false if seed in pair one is higher
 */
static bool compareByPos(const mapping::Pair &lhs,
                         const mapping::Pair &rhs) {
    return lhs.second < rhs.second;
}

void alignment::completeAlign(const FastaRecord& read,
                              const FastaRecord& reference,
                              const mapping::Band& band,
                              SmithWaterman::MutationsTupleVector& mutations) {

    // ignore empty bands, i.e. there is no no mapping between given read and reference
    if (band.empty()) {
        return;
    }

    // find first seed in mapping band
    auto first_pair = std::min_element(band.begin(), band.end(), compareByPos);
    const Seed& first_seed = first_pair->second;

    // get starting read position from the first seed
    uint32_t start_read = first_seed.getStartReadPostion();
    int32_t start_ref = static_cast<int32_t>(
                            first_seed.getStartReferencePostion() -
                            start_read - ETA);

    // check if starting reference index is out of range (lower than zero), if
    // is remember that distance and set start index to zero
    int32_t ref_out_of_range = 0;
    if(start_ref < 0) {
        ref_out_of_range = start_ref;
        start_ref = 0;
    }

    // calculate reference's sequence size
    // negative value of ref_out_of_range is added in case when starting reference
    // index is out of range, otherwise its value should be zero
    uint32_t seq_len = static_cast<uint32_t>(
                           static_cast<int32_t>(read.getSequence().size()) +
                           2 * ETA + ref_out_of_range);

    // get reference sequence segment to which read sequence is mapped
    std::string ref_seq_segment = reference.getSequence().substr(
                                      static_cast<unsigned>(start_ref),
                                      seq_len);

    SmithWaterman sw(read.getSequence(), ref_seq_segment, SW_BAND_WIDTH, false);

    // align sequences and get mutations from alignment process
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

    int32_t read_size = read_end - read_start;
    if (read_size <= 0){
        return;
    }

    int32_t ref_size = ref_end - ref_start;
    if (ref_size <= 0){
        return;
    }

    SmithWaterman sw(
        read.getSequence().substr(read_start, read_end - read_start),
        reference.getSequence().substr(ref_start, ref_end - ref_start),
        SW_BAND_WIDTH,
        false       // don't use blosum table
    );

    sw.reconstruct_path(ref_start, mutations);
}
