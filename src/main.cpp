#include <iostream>
#include "FASTA/FastaFile.h"

int main() {

	FastaFile file("../data/Bioinfo_19_20_train_data/ecoli.fasta");
	file.open();

	std::cout << "Hello World" << std::endl;
	file.close();
}
