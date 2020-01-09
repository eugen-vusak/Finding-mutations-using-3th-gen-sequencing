#include <iostream>
#include "FASTA/FastaFile.hpp"
#include "map/mapping.hpp"


#define w                           2
#define k                           2

int main() {

    // FastaFile reference_file("../data/Bioinfo_19_20_train_data/lambda.fasta");
    FastaFile reference_file("tests/data/ref.fasta");

    FastaRecord reference;
    FastaRecord::MinimizersTable reference_minimizers;

    if (reference_file.hasNextRecord()) {
        reference = reference_file.getNextRecord();
        reference_minimizers = reference.getMinimizers(k, w);
    }

    // FastaFile reads_file("../data/Bioinfo_19_20_train_data/lambda_simulated_reads.fasta");
    FastaFile reads_file("tests/data/reads.fasta");
    while (reads_file.hasNextRecord()) {
        std::cout << "New record" << std::endl;

        FastaRecord read = reads_file.getNextRecord();
        Mapping::minexmap(read, reference, reference_minimizers, w, k);

    }
}
