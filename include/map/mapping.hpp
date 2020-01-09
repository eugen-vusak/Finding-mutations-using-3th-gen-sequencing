#include "FASTA/FastaRecord.hpp"

#include "map/Seed.hpp"

// #include <set>
#include <vector>
#include <utility>

namespace mapping {

template<typename P>
struct CompareSecond {
    bool operator()(const P& firstPair, const P& secondPair) const {
        return firstPair.second < secondPair.second;
    }
};

typedef std::pair<int32_t, Seed> Pair;
typedef std::vector<mapping::Pair> Band;
// typedef std::set<Pair, CompareSecond<Pair>> Band;

Band minexmap(FastaRecord& read,
              FastaRecord& reference,
              FastaRecord::MinimizersTable& reference_minimizers,
              short w,
              short k);

} // namespace mapping

