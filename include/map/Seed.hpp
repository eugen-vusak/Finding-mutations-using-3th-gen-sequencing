#ifndef SEED_HPP
#define SEED_HPP

#include <string>
#include <cstdint>

class Seed;

// make seed hashable
namespace std {
template<>
struct hash<Seed> {
    size_t operator()(const Seed& obj) const;
};
}


class Seed {
private:
    uint32_t start_pos_read_;
    uint32_t start_pos_reference_;

    uint32_t size_read_;
    uint32_t size_reference_;

public:
    Seed() = default;
    Seed(uint32_t start_pos_read, uint32_t start_pos_reference, size_t size);

    uint32_t getStartReadPostion();
    uint32_t getStartReferencePostion();
    uint32_t getSizeRead();
    uint32_t getSizeReference();

    uint32_t extendLeft(std::string& read, std::string& reference);
    uint32_t extendRight(std::string& read, std::string& reference);
    uint32_t extendBoth(std::string& read, std::string& reference);

    bool combine(Seed& other);

    bool operator==(const Seed& other) const;
    bool operator<(const Seed& other) const;
    bool operator>(const Seed& other) const;

    friend std::ostream& operator<<(std::ostream &strm, const Seed &seed);

    friend size_t std::hash<Seed>::operator()(const Seed& obj) const;

    static void correctOrderRead(Seed* first, Seed* second);
    static void correctOrderReference(Seed* first, Seed* second);

private:
    int32_t getSharedSizeRead_(const Seed& other);
    int32_t getSharedSizeReference_(const Seed& other);
};

#endif // !SEED_HPP
