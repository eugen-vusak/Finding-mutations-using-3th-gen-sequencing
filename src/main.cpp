#include "FASTA/FastaFile.hpp"
#include "map/mapping.hpp"
#include "align/alignment.hpp"
#include "config/Config.hpp"

#include <iostream>
#include <fstream>


#define W_DEFAULT   12
#define K_DEFAULT   12

static void print_mutations(const SmithWaterman::MutationsTupleVector& mutations, std::ostream& out);

int main() {

    // init config
    Config::setProjectPath(get_project_path().string());
    int error = load_config();
    if (error) {
        return -1;
    }

    // get parameters for minimizers computation
    short w = Config::getInstance().get("minimizers_w", W_DEFAULT);
    short k = Config::getInstance().get("minimizers_k", K_DEFAULT);

    // get filenames
    std::string reference_filename;
    Config::getInstance().get_to("program_reference_filename", reference_filename);
    std::string reads_filename;
    Config::getInstance().get_to("program_reads_filename", reads_filename);
    std::string output_filename;
    Config::getInstance().get_to("program_output_filename", output_filename);

    // reference
    FastaFile reference_file(reference_filename);

    FastaRecord reference;
    FastaRecord::MinimizersTable reference_minimizers;

    if (reference_file.hasNextRecord()) {
        reference = reference_file.getNextRecord();
        reference_minimizers = reference.getMinimizers(k, w);
    } else {
        // thorw exception
    }

    std::ofstream output_file;
    output_file.open(output_filename);

    // reads
    FastaFile reads_file(reads_filename);

    while (reads_file.hasNextRecord()) {
        FastaRecord read = reads_file.getNextRecord();
        std::cout << read.getHeader() << std::endl;

        // mapping
        mapping::Band band = mapping::minexmap(read, reference, reference_minimizers, w, k);

        // alignment
        SmithWaterman::MutationsTupleVector mutations;
        alignment::completeAlign(read, reference, band, mutations);
        print_mutations(mutations, output_file);
    }

    output_file.close();
}

static void print_mutations(const SmithWaterman::MutationsTupleVector& mutations, std::ostream& out) {
    for(auto mut : mutations) {
        out << std::get<0>(mut) << ",";
        out << std::get<1>(mut) << ",";
        out << std::get<2>(mut) << std::endl;
    }
}
