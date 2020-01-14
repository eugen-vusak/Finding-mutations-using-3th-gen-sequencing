#include "FASTA/FastaRecord.hpp"
#include "align/SmithWaterman.hpp"
#include "map/mapping.hpp"

namespace alignment {

void completeAlign(const FastaRecord& read,
                   const FastaRecord& reference,
                   const mapping::Band& band,
                   SmithWaterman::MutationsTupleVector& mutations);

/**
 * @brief Aligns read to reference but uses partial read
 *
 * For alignment only part of read that is after first minimizer
 * and before last minimizer is used.
 *
 * Example:
 * read:    ____FFFF______LLL_____
 * used:    --------+++++++++-----
 *
 * - where F marks first minimizer and L marks last minimizer
 *   and where - marks unused portion of read and + marks used portion
 *
 * @param read that will be aligned
 * @param reference to which read will be aligned
 * @param band mapping band that tells where read maps to reference
 * @param mutations rezulting mutations from alignment process
 */
void partialAlign(const FastaRecord& read,
                  const FastaRecord& reference,
                  const mapping::Band& band,
                  SmithWaterman::MutationsTupleVector& mutations);

}
