#include "FASTA/FastaFile.h"

#include <iostream>

FastaFile::FastaFile(const std::string& filename)
    : filename_(filename),
      opened_(false),
      has_next_record_(false),
      next_header_()
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

    readHeader();

    return true;
}

void FastaFile::close() {

    if (! opened_) {
        return;
    }

    file_stream_.close();
    opened_ = false;
}

void FastaFile::readHeader() {

    std::string line;
    while( std::getline( file_stream_, line ).good()) {

        if (line[0] == '>') {
            next_header_ = line.substr(1);
            has_next_record_ = true;
            break;
        }
    }
}

bool FastaFile::hasNextRecord() {
    return has_next_record_;
}

FastaRecord FastaFile::getNextRecord() {

    if (! has_next_record_) {
        throw std::out_of_range("No more records");
    }

    FastaRecord record;
    record.setHeader(next_header_);
    has_next_record_ = false;

    std::string line;
    while( std::getline( file_stream_, line ).good()) {

        if (line[0] == '>') {
            next_header_ = line;
            has_next_record_ = true;
            break;
        }

        record.extendSequence(line);
    }

    return record;
}
