#include "map/Seed.hpp"

#include <iostream>

Seed::Seed(uint32_t start_pos_read, uint32_t start_pos_reference, size_t size)
    :start_pos_read_(start_pos_read),
     start_pos_reference_(start_pos_reference),
     size_read_(static_cast<uint32_t>(size)),
     size_reference_(static_cast<uint32_t>(size)) {

}

uint32_t Seed::getStartReadPostion() {
    return start_pos_read_;
}

uint32_t Seed::getStartReferencePostion() {
    return start_pos_reference_;
}

uint32_t Seed::getSizeRead() {
    return size_read_;
}

uint32_t Seed::getSizeReference() {
    return size_reference_;
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
    size_read_ += extension_size;
    size_reference_ += extension_size;
    return extension_size;
}

uint32_t Seed::extendRight(std::string& read, std::string& reference) {

    auto read_iter = read.begin() + static_cast<unsigned>(start_pos_read_ + size_read_);
    auto reference_iter = reference.begin() + static_cast<unsigned>(start_pos_reference_ + size_reference_);

    uint32_t extension_size = 0;
    while (read_iter < read.end() && reference_iter < reference.end()) {

        if (*read_iter != *reference_iter) {
            break;
        }

        extension_size++;

        ++read_iter;
        ++reference_iter;
    }
    size_read_ += extension_size;
    size_reference_ += extension_size;
    return extension_size;
}

uint32_t Seed::extendBoth(std::string& read, std::string& reference) {
    uint32_t extension_size = 0u;
    extension_size += extendLeft(read, reference);
    extension_size += extendRight(read, reference);

    return extension_size;
}

bool Seed::combine(Seed& other) {

    Seed* first_read = this;
    Seed* second_read = &other;
    Seed* first_reference= this;
    Seed* second_reference = &other;

    correctOrderRead(first_read, second_read);
    int32_t shared_read = first_read->start_pos_read_ + first_read->size_read_ - second_read->start_pos_read_;

    if (shared_read < 0) {
        return false;
    }

    correctOrderReference(first_reference, second_reference);
    int32_t shared_reference = first_reference->start_pos_reference_ + first_reference->size_reference_
                               - second_reference->start_pos_reference_;
    if (shared_reference < 0) {
        return false;
    }

    size_read_ = first_read->size_read_ + second_read->size_read_ - shared_read;
    start_pos_read_ = first_read->start_pos_read_;

    size_reference_ = first_reference->size_reference_ + second_reference->size_reference_ - shared_reference;
    start_pos_reference_ = first_reference->start_pos_reference_;

    return true;
}

void Seed::correctOrderRead(Seed* first, Seed* second) {
    if (first->start_pos_read_ < second->start_pos_read_) {
        return;
    }

    Seed* tmp;
    if (first->start_pos_read_ > second->start_pos_read_) {
        tmp = first;
        first = second;
        second = tmp;
        return;
    }
    if (first->size_read_ > second->size_read_) {
        tmp = first;
        first = second;
        second = tmp;
    }

    return;
}

void Seed::correctOrderReference(Seed* first, Seed* second) {
    if (first->start_pos_reference_ < second->start_pos_reference_) {
        return;
    }

    Seed* tmp;
    if (first->start_pos_reference_ > second->start_pos_reference_) {
        tmp = first;
        first = second;
        second = tmp;
        return;
    }
    if (first->size_reference_ > second->size_reference_) {
        tmp = first;
        first = second;
        second = tmp;
    }

    return;
}

bool Seed::operator==(const Seed& other) const {
    return (start_pos_read_ == other.start_pos_read_ &&
            start_pos_reference_ == other.start_pos_reference_ &&
            size_read_ == other.size_read_ &&
            size_reference_ == other.size_reference_);
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
    strm << ", " << seed.size_read_;
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
    hash_combine(seed, obj.size_read_);
    hash_combine(seed, obj.size_reference_);
    return seed;
}
