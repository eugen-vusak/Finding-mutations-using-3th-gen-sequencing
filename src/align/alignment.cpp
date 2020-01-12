#include "align/alignment.hpp"


#include <iostream>
#include <algorithm>
#include <set>

#define ETA 10


static bool compareByPos(const mapping::Pair &lhs, const mapping::Pair &rhs) {
    return lhs.second < rhs.second;
}

void alignment::alignRead(FastaRecord& reference, FastaRecord& read, mapping::Band band,
                          SmithWaterman::MutationsTupleSet& mutations) {

    if (band.empty()) {
        return;
    }

    std::vector<mapping::Pair>::iterator first_pair = std::min_element(band.begin(), band.end(), compareByPos);
    Seed first_seed = first_pair->second;

    uint32_t start_read = first_seed.getStartReadPostion();
    int32_t start_ref = first_seed.getStartReferencePostion() - start_read - ETA;

    int32_t ref_out_of_range = 0;
    if(start_ref < 0) {
        ref_out_of_range = start_ref;
        start_ref = 0;
    }

    uint32_t seq_len = read.getSequence().size() + 2 * ETA + ref_out_of_range;
    std::string ref_seq_segment = reference.getSequence().substr(start_ref, seq_len);

    std::cout << "->" << std::endl;

    bool blosum_use = false;
    SmithWaterman sw(read.getSequence(), ref_seq_segment, blosum_use);

    sw.reconstruct_path(start_ref, mutations);
}
