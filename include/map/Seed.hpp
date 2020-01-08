#include <string>

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
    size_t start_pos_read_;
    size_t start_pos_reference_;

    size_t size_;

    size_t end_pos_read_;
    size_t end_pos_reference_;

public:
    Seed() = default;
    Seed(size_t start_pos_read, size_t start_pos_reference, size_t size);

    size_t extendLeft(std::string& read, std::string& reference);
    size_t extendRight(std::string& read, std::string& reference);
    size_t extendBoth(std::string& read, std::string& reference);

    bool operator==(const Seed& other) const;

    friend std::ostream& operator<<(std::ostream &strm, const Seed &seed);

    friend size_t std::hash<Seed>::operator()(const Seed& obj) const;
};
