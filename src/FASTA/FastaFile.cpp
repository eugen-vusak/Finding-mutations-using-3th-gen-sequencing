#include "FASTA/FastaFile.h"

#include <iostream>

FastaFile::FastaFile(const std::string& filename)
    : filename_(filename),
      opened_(false)
{}

FastaFile::~FastaFile() {
    close();
}

bool FastaFile::open() {
    file_stream_ = std::ifstream(filename_);

    if(!file_stream_.good()) {
        std::cerr << "Error opening '"<< filename_ << "'" << std::endl;
        return false;
    }

    opened_ = true;

    return true;
}

void FastaFile::close() {

    if (! opened_) {
        return;
    }

    file_stream_.close();
    opened_ = false;
}

