#include <iostream>
#include "FASTA/FastaFile.h"

int main() {

    FastaFile file("../data/Bioinfo_19_20_train_data/ecoli.fasta");
    file.open();

    while (file.hasNextRecord()) {
        std::cout << "New record" << std::endl;
        std::cout << file.getNextRecord() << std::endl;
    }

    file.close();
}
