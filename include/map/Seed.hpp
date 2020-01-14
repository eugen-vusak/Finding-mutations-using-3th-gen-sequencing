#ifndef SEED_HPP
#define SEED_HPP

#include <string>
#include <cstdint>

class Seed;

// for making Seed class hashable
namespace std {

template<>
struct hash<Seed> {
    size_t operator()(const Seed& obj) const;
};
}

/**
 * @brief Describes Seed object used for mapping sequences
 *
 * Seed object is abstraction of a match found on both read and reference sequence.
 * It describes their link
 */
class Seed {
private:
    uint32_t start_pos_read_;       // start position of seed on read sequence
    uint32_t start_pos_reference_;  // start position of seed on reference sequence

    uint32_t size_read_;            // size of seed on read sequence
    uint32_t size_reference_;       // size of read on reference sequence

public:
    /**
     * @brief Construct a new Seed object
     */
    Seed() = default;

    /**
     * @brief Construct a new Seed object from positions and size
     *
     * @param start_pos_read start position of seed on read sequence
     * @param start_pos_reference start position of seed on reference sequence
     * @param size size of a seed
     */
    Seed(uint32_t start_pos_read, uint32_t start_pos_reference, size_t size);

    /**
     * @brief Get the start position of seed on read sequence
     *
     * @return uint32_t
     */
    uint32_t getStartReadPostion() const;
    /**
     * @brief Get the start position of seed on reference sequence
     *
     * @return uint32_t
     */
    uint32_t getStartReferencePostion() const;
    /**
     * @brief Get the size of seed on read sequence
     *
     * @return uint32_t
     */
    uint32_t getSizeRead() const;
    /**
     * @brief Get the size of seed on reference sequence
     *
     * @return uint32_t
     */
    uint32_t getSizeReference() const;

    /**
     * @brief Extends seed to left
     *
     * Extends seed to the left by iterating backwards on both
     * read and reference sequences and while they are matching
     * start position on both is moved and seed size is increased
     * Function returns size of extension that occured
     *
     * @param read sequence of a read
     * @param reference sequence of a reference
     * @return uint32_t
     */
    uint32_t extendLeft(const std::string& read, const std::string& reference);
    /**
     * @brief Extends seed to right
     *
     * Extends seed to the right by iterating forwards on both
     * read and reference sequences and while they are matching
     * start position on both is moved and seed size is increased
     * Function returns size of extension that occured
     *
     * @param read sequence of a read
     * @param reference sequence of a reference
     * @return uint32_t
     */
    uint32_t extendRight(const std::string& read, const std::string& reference);
    /**
     * @brief Extends seed to both directions
     *
     * Extends seed to the both directions by first iterating
     * backwards on both read and reference sequences and than
     * iterating forwards as well. While sequences are matching
     * start position on both is moved and seed size is increased
     * Function returns size of extension that occured
     *
     * @see extendRight
     * @see extendLeft
     *
     * @param read sequence of a read
     * @param reference sequence of a reference
     * @return uint32_t
     */
    uint32_t extendBoth(const std::string& read, const std::string& reference);

    bool combine(Seed& other);

    bool operator==(const Seed& other) const;
    bool operator<(const Seed& other) const;
    bool operator>(const Seed& other) const;

    friend std::ostream& operator<<(std::ostream &strm, const Seed &seed);

    friend size_t std::hash<Seed>::operator()(const Seed& obj) const;

    static void correctOrderRead(Seed** first, Seed** second);
    static void correctOrderReference(Seed** first, Seed** second);

private:
    int32_t getSharedSizeRead_(const Seed& other) const;
    int32_t getSharedSizeReference_(const Seed& other) const;
};

#endif // !SEED_HPP
