#include "FASTA/FastaRecord.hpp"
#include "FASTA/FastaFile.hpp"
#include "align/SmithWaterman.hpp"
#include "map/mapping.hpp"


namespace alignment {

void alignRead(FastaRecord& reference, FastaRecord& read, mapping::Band band,
               SmithWaterman::MutationsTupleVector& mutations);

}
