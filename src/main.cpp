#include "FASTA/FastaFile.hpp"
#include "map/mapping.hpp"
#include "align/alignment.hpp"

#include <iostream>
#include <stdio.h>
#include <algorithm>

#define w   12
#define k   12


int main() {

    FastaFile reference_file("../data/Bioinfo_19_20_train_data/lambda.fasta");
    //FastaFile reference_file("tests/data/ref.fasta");
    //FastaFile reference_file("../data/reference.fasta");

    FastaRecord reference;
    FastaRecord::MinimizersTable reference_minimizers;

    if (reference_file.hasNextRecord()) {
        reference = reference_file.getNextRecord();
        reference_minimizers = reference.getMinimizers(k, w);
    } else {
        // thorw exception
    }

    FastaFile reads_file("../data/Bioinfo_19_20_train_data/lambda_simulated_reads.fasta");
    //FastaFile reads_file("tests/data/reads.fasta");
    //FastaFile reads_file("../data/read.fasta");

    int n = 0;
    SmithWaterman::MutationsTupleSet mutations;
    while (reads_file.hasNextRecord()) {
        std::cout << n << ": " << std::endl;

        FastaRecord read = reads_file.getNextRecord();
        mapping::Band band = mapping::minexmap(read, reference, reference_minimizers, w, k);

        alignment::alignRead(reference, read, band, mutations);

        if (n++ == 15) break;
    }

    for(auto mut : mutations) {
        std::cout << std::get<0>(mut) << "," << std::get<1>(mut) << "," << std::get<2>(mut) << std::endl;
    }
}
