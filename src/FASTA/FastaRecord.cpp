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

std::vector<std::string> FastaRecord::getMinimizers(short k, short w) {
    std::vector<std::string> minimizers;
    int window_size = w + k - 1;

    std::string::iterator seq_start = sequence_.begin();

    while(true) {
        std::string::iterator win_start = seq_start;
        std::string::iterator win_end = win_start + window_size;

        std::string::iterator min_i = win_start;
        std::string min = std::string(min_i, min_i + k);

        while(win_start < win_end - k + 1) {
            std::string kmer = std::string(win_start, win_start + k); //??

            if(kmer < min) {
                min = kmer;
            }

            win_start ++;
        }

        minimizers.push_back(min);
        seq_start ++;

        if(seq_start >= sequence_.end() - window_size + 1){
            break;
        }
    }

    return minimizers;
}

std::ostream& operator<<(std::ostream &strm, const FastaRecord &record) {
    // string representation of object (only first 40 letters of sequence are printed)
    strm << record.header_ << std::endl;
    strm << record.sequence_.substr(0, 40) << "...";
    return strm;
}
