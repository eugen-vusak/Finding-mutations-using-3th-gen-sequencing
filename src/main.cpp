#include "FASTA/FastaFile.hpp"
#include "map/mapping.hpp"
#include "align/alignment.hpp"

#include <iostream>

#define w   12
#define k   12

static void print_mutations(const SmithWaterman::MutationsTupleVector& mutations);

int main() {

    FastaFile reference_file("../data/Bioinfo_19_20_train_data/lambda.fasta");
    //FastaFile reference_file("tests/data/ref.fasta");
    //FastaFile reference_file("../data/reference.fasta");
    // FastaFile reference_file("tests/data/random_ref.fasta");

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
    // FastaFile reads_file("tests/data/random_reads.fasta");

    int n = 0;
    while (reads_file.hasNextRecord()) {
        FastaRecord read = reads_file.getNextRecord();
        std::cout << read.getHeader() << std::endl;

        // mapping
        mapping::Band band = mapping::minexmap(read, reference, reference_minimizers, w, k);

        // alignment
        SmithWaterman::MutationsTupleVector mutations;
        alignment::completeAlign(read, reference, band, mutations);
        print_mutations(mutations);

        if (n++ == 15) break;
    }
}

static void print_mutations(const SmithWaterman::MutationsTupleVector& mutations) {
    for(auto mut : mutations) {
        std::cout << std::get<0>(mut) << ",";
        std::cout << std::get<1>(mut) << ",";
        std::cout << std::get<2>(mut) << std::endl;
    }
}
