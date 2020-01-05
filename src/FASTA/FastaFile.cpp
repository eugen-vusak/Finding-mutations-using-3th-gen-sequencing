#include "FASTA/FastaFile.h"

#include <iostream>

FastaFile::FastaFile(const std::string& filename)
    : filename_(filename),
      has_next_record_(false),
      next_header_()
{
    open();
}

FastaFile::~FastaFile() {
    close();
}

bool FastaFile::open() {
    // open input file stream
    file_stream_ = std::ifstream(filename_);

    if(!file_stream_.good()) {
        std::cerr << "Error opening '"<< filename_ << "'" << std::endl;
        return false;
    }

    // read first header to see if any records exist
    readHeader();

    return true;
}

void FastaFile::close() {

    if (! file_stream_.is_open()) {
        return;
    }

    // close stream and clear has_next_record flag
    has_next_record_ = false;
    file_stream_.close();
}

void FastaFile::readHeader() {
    // read and ignore all lines until header line
    // when it's reached store it and mark existance of next record

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

    // if file is closed warn user
    if (! file_stream_.is_open()) {
        std::cout << "WARNNING: File is closed" << std::endl;
    }

    return has_next_record_;
}

FastaRecord FastaFile::getNextRecord() {

    if (!file_stream_.is_open()) {
        throw std::runtime_error("File is closed");
    }

    if (! has_next_record_) {
        throw std::out_of_range("No more records. See hasNextRecord()");
    }

    FastaRecord record;

    // set cached header line and clear has_next_record flag
    record.setHeader(next_header_);
    has_next_record_ = false;

    std::string line;
    while( std::getline( file_stream_, line ).good()) {

        // if line is header (starts with '>')
        // save header line and mark existance of next record
        if (line[0] == '>') {
            next_header_ = line;
            has_next_record_ = true;
            break;
        }

        // else line is sequence line of record thus extend current sequence with it
        record.extendSequence(line);
    }

    return record;
}
