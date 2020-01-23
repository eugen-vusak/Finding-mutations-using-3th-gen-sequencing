// #include <cstddef>
#include <functional>

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
void hash_combine(std::size_t& seed, const T& v) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}
