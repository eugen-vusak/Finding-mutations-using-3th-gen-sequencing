#include "FASTA/FastaRecord.h"

#include "iostream"

FastaRecord::FastaRecord()
    :sequence_(),
     header_()
{}

void FastaRecord::setHeader(const std::string& header) {
    header_ = header;
}

void FastaRecord::extendSequence(const std::string& sequence) {
    sequence_ += sequence;
}

std::ostream& operator<<(std::ostream &strm, const FastaRecord &record) {
    strm << record.header_ << std::endl;
    strm << record.sequence_.substr(0, 40) << "...";
    return strm;
}
