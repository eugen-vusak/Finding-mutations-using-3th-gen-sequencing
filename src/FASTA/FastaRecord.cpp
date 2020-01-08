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

// The main part of the function are two nested loops. Outer while loop iterates through
// sequence with consecutive windows, while inner for loop iterates through given windows and
// computes k-mers. At the end of the inner loop a minimizers is found for a given window.
// In both cases consecutive means shifted by one.
FastaRecord::MinimizersTable FastaRecord::getMinimizers(short k, short w) {

    MinimizersTable minimizers;

    std::string::iterator win_start = sequence_.begin();
    while(true) {

        // initializes window's minimizer as first k-mer
        std::string::iterator min_i = win_start;
        std::string min = std::string(min_i, min_i + k);

        // finds minimizer for window (starting from second k-mer)
        auto win_pos = win_start + 1;
        for (; win_pos < win_start + w; ++win_pos) {

            std::string kmer = std::string(win_pos, win_pos + k);

            if(kmer < min) {
                min_i = win_pos;
                min = kmer;
            }
        }

        unsigned int pos = static_cast<unsigned int>(std::distance(sequence_.begin(), min_i));
        minimizers[min].insert(pos);

        // stops if end of k-mer is at the end of sequence
        if(win_pos + k - 1 >= sequence_.end()) {
            break;
        }

        ++win_start;
    }

    return minimizers;
}

// string representation of object (only first 40 letters of sequence are printed)
std::ostream& operator<<(std::ostream &strm, const FastaRecord &record) {
    strm << record.header_ << std::endl;
    strm << record.sequence_.substr(0, 40) << "...";
    return strm;
}
