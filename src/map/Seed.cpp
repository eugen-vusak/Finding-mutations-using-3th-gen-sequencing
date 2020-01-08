#include "map/Seed.hpp"

#include <iostream>

Seed::Seed(size_t start_pos_read, size_t start_pos_reference, size_t size)
    :start_pos_read_(start_pos_read),
     start_pos_reference_(start_pos_reference),
     size_(size),
     end_pos_read_(start_pos_read + size),
     end_pos_reference_(start_pos_reference + size)
{}

size_t Seed::extendLeft(std::string& read, std::string& reference) {

    auto read_iter = read.begin() + static_cast<unsigned>(start_pos_read_) - 1;
    auto reference_iter = reference.begin() + static_cast<unsigned>(start_pos_reference_) - 1;

    size_t extension_size = 0;
    while (read_iter >= read.begin() and reference_iter >= reference.begin()) {

        if (*read_iter != *reference_iter) {
            break;
        }

        start_pos_read_--;
        start_pos_reference_--;

        extension_size++;

        --read_iter;
        --reference_iter;
    }
    size_ += extension_size;
    return extension_size;
}

size_t Seed::extendRight(std::string& read, std::string& reference) {

    auto read_iter = read.begin() + static_cast<unsigned>(end_pos_read_);
    auto reference_iter = reference.begin() + static_cast<unsigned>(end_pos_reference_);

    size_t extension_size = 0;
    while (read_iter < read.end() and reference_iter < reference.end()) {

        if (*read_iter != *reference_iter) {
            break;
        }

        end_pos_read_++;
        end_pos_reference_++;

        extension_size++;

        ++read_iter;
        ++reference_iter;
    }
    size_ += extension_size;
    return extension_size;
}

size_t Seed::extendBoth(std::string& read, std::string& reference) {
    size_t extension_size = 0u;
    extension_size += extendLeft(read, reference);
    extension_size += extendRight(read, reference);

    return extension_size;
}

}

std::ostream& operator<<(std::ostream &strm, const Seed &seed) {
    strm << "Seed(" << seed.start_pos_read_ << " <-> " << seed.start_pos_reference_;
    // strm << ", " << seed.start_pos_reference_ << " -> " << seed.end_pos_reference_;
    strm << ", " << seed.size_;
    return strm << ")";
}
