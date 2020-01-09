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
public:
    uint32_t start_pos_read_;
    uint32_t start_pos_reference_;

    uint32_t size_;

public:
    Seed() = default;
    Seed(uint32_t start_pos_read, uint32_t start_pos_reference, size_t size);

    uint32_t extendLeft(std::string& read, std::string& reference);
    uint32_t extendRight(std::string& read, std::string& reference);
    uint32_t extendBoth(std::string& read, std::string& reference);

    bool operator==(const Seed& other) const;
    bool operator<(const Seed& other) const;
    bool operator>(const Seed& other) const;

    friend std::ostream& operator<<(std::ostream &strm, const Seed &seed);

    friend size_t std::hash<Seed>::operator()(const Seed& obj) const;
};
