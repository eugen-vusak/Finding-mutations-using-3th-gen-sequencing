#ifndef ALIGNMENT_HPP
#define ALIGNMENT_HPP

#include "FASTA/FastaRecord.hpp"
#include "align/SmithWaterman.hpp"
#include "map/mapping.hpp"

namespace alignment {

/**
 * @brief Aligns whole read to reference
 * 
 * Finds mapping of whole read on reference and aligns it with Smith-Waterman 
 * algorithm. Whole read sequence and its reference mapping sequence are used.
 * Starting index on reference is found from first seed in given band, which is 
 * then shifted for corresponding distance of starting index on the read sequence.
 * 
 * Example:
 * read:    ____FFFF______LLL_____
 * used:    ++++++++++++++++++++++
 *
 * - where F marks first minimizer and L marks last minimizer
 *   and where - marks unused portion of read and + marks used portio
 * 
 * @param read that will be aligned
 * @param reference to which read will be aligned
 * @param band mapping band that tells where read maps to reference
 * @param mutations resulting mutations from alignment process
 */
void completeAlign(const FastaRecord& read,
                   const FastaRecord& reference,
                   const mapping::Band& band,
                   SmithWaterman::MutationsTupleVector& mutations);

/**
 * @brief Aligns read to reference but uses partial read
 *
 * For alignment, only part of read that is after first minimizer
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

#endif // !ALIGNMENT_HPP
