#include "map/Seed.hpp"

#include <iostream>

Seed::Seed(unsigned int start_pos_read, unsigned int start_pos_reference, unsigned int k)
    :start_pos_read_(start_pos_read),
     start_pos_reference_(start_pos_reference),
     end_pos_read_(start_pos_read + k),
     end_pos_reference_(start_pos_reference + k)
{}

void Seed::extendLeft(std::string& read, std::string& reference) {

    auto read_iter = read.begin() + start_pos_read_;
    auto reference_iter = reference.begin() + start_pos_reference_;

    while (read_iter > read.begin() and reference_iter > reference.begin()) {

        if (*read_iter != *reference_iter) {
            break;
        }

        start_pos_read_--;
        start_pos_reference_--;

        --read_iter;
        --reference_iter;
    }
}

void Seed::extendRight(std::string& read, std::string& reference) {

    auto read_iter = read.begin() + end_pos_read_;
    auto reference_iter = reference.begin() + end_pos_reference_;

    while (read_iter < read.end() and reference_iter < reference.end()) {

        if (*read_iter != *reference_iter) {
            break;
        }

        end_pos_read_++;
        end_pos_reference_++;

        ++read_iter;
        ++reference_iter;
    }
}

void Seed::extendBoth(std::string& read, std::string& reference) {
    extendLeft(read, reference);
    extendRight(read, reference);
}

std::ostream& operator<<(std::ostream &strm, const Seed &seed) {
    strm << "Seed(" << seed.start_pos_read_ << " -> " << seed.end_pos_read_;
    strm << ", " << seed.start_pos_reference_ << " -> " << seed.end_pos_reference_;
    return strm << ")";
}
