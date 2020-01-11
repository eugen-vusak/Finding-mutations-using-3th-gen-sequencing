#include "map/Seed.hpp"

#include <iostream>

Seed::Seed(uint32_t start_pos_read, uint32_t start_pos_reference, size_t size)
    :start_pos_read_(start_pos_read),
     start_pos_reference_(start_pos_reference),
     size_(static_cast<uint32_t>(size)) {
}

uint32_t Seed::getStartReadPostion() {
    return start_pos_read_;
}

uint32_t Seed::getStartReferencePostion() {
    return start_pos_reference_;
}

uint32_t Seed::getSize() {
    return size_;
}

uint32_t Seed::extendLeft(const std::string& read, const std::string& reference) {

    // get staring point on both sequences
    // starting point is one letter before current first letter
    auto read_iter = read.begin() + static_cast<unsigned>(start_pos_read_) - 1;
    auto reference_iter = reference.begin() + static_cast<unsigned>(start_pos_reference_) - 1;

    uint32_t extension_size = 0;
    // stop if any sequence has reached it's begining
    while (read_iter >= read.begin() && reference_iter >= reference.begin()) {

        // stop if letters are not matched
        if (*read_iter != *reference_iter) {
            break;
        }
        // else (they are matched) extend seed

        // shitf start position to left
        start_pos_read_--;
        start_pos_reference_--;

        // increase extension size
        extension_size++;

        // advance to next left letter
        --read_iter;
        --reference_iter;
    }

    // finally update size of a seed
    size_ += extension_size;
    return extension_size;
}

uint32_t Seed::extendRight(const std::string& read, const std::string& reference) {

    // get staring point on both sequences
    // starting point is one letter after current last letter
    auto read_iter = read.begin() + static_cast<unsigned>(start_pos_read_ + size_);
    auto reference_iter = reference.begin() + static_cast<unsigned>(start_pos_reference_ + size_);

    uint32_t extension_size = 0;
    // stop if any sequence has reached it's end
    while (read_iter < read.end() && reference_iter < reference.end()) {

        // stop if letters are not matched
        if (*read_iter != *reference_iter) {
            break;
        }
        // else (they are matched) extend seed

        // increase extension size
        extension_size++;

        // advance to next right letter
        ++read_iter;
        ++reference_iter;
    }

    // finally update size of a seed
    size_ += extension_size;
    return extension_size;
}

uint32_t Seed::extendBoth(const std::string& read, const std::string& reference) {
    // to extend sequence to both sides extend first to left and than to right
    // total extension size is sum of sizes of both left and right extension size

    uint32_t extension_size = extendLeft(read, reference);
    extension_size += extendRight(read, reference);

    return extension_size;
}

bool Seed::operator==(const Seed& other) const {
    return (start_pos_read_ == other.start_pos_read_ &&
            size_ == other.size_);
}

bool Seed::operator<(const Seed& other) const {
    return start_pos_reference_ < other.start_pos_reference_;
}

bool Seed::operator>(const Seed& other) const {
    return start_pos_reference_ > other.start_pos_reference_;
}

std::ostream& operator<<(std::ostream &strm, const Seed &seed) {
    strm << "Seed(" << seed.start_pos_read_ << " <-> " << seed.start_pos_reference_;
    // strm << ", " << seed.start_pos_reference_ << " -> " << seed.end_pos_reference_;
    strm << ", " << seed.size_;
    return strm << ")";
}

/**
 * @brief Combines multiple values into one hash
 *
 * Set first seed to any value, for example 0. Than call
 * hash_combine with that seed and first of few values.
 * Resulting value should be used as seed for next of few values
 * that are to be combined
 *
 * Based on boost::hash_combine
 * @see https://www.boost.org/doc/libs/1_35_0/doc/html/boost/hash_combine_id241013.html
 *
 * @tparam T type of object to be hashed
 * @param seed result of last hash_combine or initial value
 * @param v object to be hashed
 */
template <typename T>
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
