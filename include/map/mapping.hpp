#ifndef MAPPING_HPP
#define MAPPING_HPP

#include "map/Seed.hpp"
#include "FASTA/FastaRecord.hpp"

// #include <set>
#include <vector>
#include <utility>

namespace mapping {

// template<typename P>
// struct CompareSecond {
//     bool operator()(const P& firstPair, const P& secondPair) const {
//         return firstPair.second < secondPair.second;
//     }
// };

/**
 * hit pair consisting of diff between start position on read and on reference
 * and of seed descripting that hit
 */
typedef std::pair<int32_t, Seed> Pair;

/**
 * @brief One band from all possible hit pairs
 *
 * Band marks one approximately colinear subset of all hits. On
 * alignment matrix it depicts one diagonal band of seed hits that are
 * less than epsilon away.
 */
typedef std::vector<mapping::Pair> Band;
// typedef std::set<Pair, CompareSecond<Pair>> Band;

/**
 * @brief Maps read to reference using minimizers
 *
 * Preformes mapping of read sequence to reference sequence
 * by mapping read minimizers to reference minimizers. It does
 * that by first finding hits (exact matches) of minimizers on both
 * sequences and from all those hits finds maximal approximately
 * colinear subset. Maximal subset is one with longest lenght.
 *
 * @cite 10.1093/bioinformatics/btw152
 *
 * @param read read object that will be mached
 * @param reference_minimizers minimizers calculated for reference object
 * @param w window size for minimizers computations
 * @param k size of minimizer
 * @return Band maximal approximately colinear subset
 */
Band minimap(FastaRecord& read,
             FastaRecord::MinimizersTable& reference_minimizers,
             short w,
             short k);

/**
 * @brief Maps read to reference using extended minimizers
 *
 * Preformes mapping of read sequence to reference sequence
 * by mapping read minimizers to reference minimizers. It does
 * that by first finding hits (exact matches) of minimizers on both
 * sequences. Those seeds are then extended and from all those
 * extended seeds function finds maximal approximately
 * colinear subset. Maximal subset is one with longest lenght.
 *
 * @see minimap
 * @cite 10.1093/bioinformatics/btw152
 *
 * @param read read object that will be mached
 * @param reference reference object on which read will be matched
 * @param reference_minimizers minimizers calculated for reference object
 * @param w window size for minimizers computations
 * @param k size of minimizer
 * @return Band maximal approximately colinear subset
 */
Band minexmap(FastaRecord& read,
              FastaRecord& reference,
              FastaRecord::MinimizersTable& reference_minimizers,
              short w,
              short k);

} // namespace mapping

#endif //  MAPPING_HPP
