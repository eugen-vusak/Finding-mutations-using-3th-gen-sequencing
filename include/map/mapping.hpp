#include "FASTA/FastaRecord.hpp"

namespace Mapping {

void minimap(FastaRecord& read,
             FastaRecord& reference,
             FastaRecord::MinimizersTable& reference_minimizers,
             short w,
             short k);

void minexmap(FastaRecord& read,
              FastaRecord& reference,
              FastaRecord::MinimizersTable& reference_minimizers,
              short w,
              short k);

} // namespace mapping

