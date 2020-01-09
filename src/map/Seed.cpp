#include "map/Seed.hpp"

#include <iostream>

Seed::Seed(uint32_t start_pos_read, uint32_t start_pos_reference, size_t size)
    :start_pos_read_(start_pos_read),
     start_pos_reference_(start_pos_reference),
     size_(static_cast<uint32_t>(size)) {

}

uint32_t Seed::extendLeft(std::string& read, std::string& reference) {

    auto read_iter = read.begin() + static_cast<unsigned>(start_pos_read_) - 1;
    auto reference_iter = reference.begin() + static_cast<unsigned>(start_pos_reference_) - 1;

    uint32_t extension_size = 0;
    while (read_iter >= read.begin() && reference_iter >= reference.begin()) {

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

uint32_t Seed::extendRight(std::string& read, std::string& reference) {

    auto read_iter = read.begin() + static_cast<unsigned>(start_pos_read_ + size_);
    auto reference_iter = reference.begin() + static_cast<unsigned>(start_pos_reference_ + size_);

    uint32_t extension_size = 0;
    while (read_iter < read.end() && reference_iter < reference.end()) {

        if (*read_iter != *reference_iter) {
            break;
        }

        extension_size++;

        ++read_iter;
        ++reference_iter;
    }
    size_ += extension_size;
    return extension_size;
}

uint32_t Seed::extendBoth(std::string& read, std::string& reference) {
    uint32_t extension_size = 0u;
    extension_size += extendLeft(read, reference);
    extension_size += extendRight(read, reference);

    return extension_size;
}

bool Seed::operator==(const Seed& other) const {
    return (start_pos_read_ == other.start_pos_read_ &&
            size_ == other.size_);
}


std::ostream& operator<<(std::ostream &strm, const Seed &seed) {
    strm << "Seed(" << seed.start_pos_read_ << " <-> " << seed.start_pos_reference_;
    // strm << ", " << seed.start_pos_reference_ << " -> " << seed.end_pos_reference_;
    strm << ", " << seed.size_;
    return strm << ")";
}

template <class T>
inline void hash_combine(std::size_t& seed, const T& v) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

size_t std::hash<Seed>::operator()(const Seed& obj) const {
    size_t seed = 0;
    hash_combine(seed, obj.start_pos_read_);
    hash_combine(seed, obj.start_pos_reference_);
    hash_combine(seed, obj.size_);
    return seed;
}
