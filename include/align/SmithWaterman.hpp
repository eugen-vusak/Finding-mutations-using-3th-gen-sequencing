#include "align/blosum.hpp"

#include <string>
#include <vector>
#include <tuple>
#include <set>


struct mutationTuple_comparator {
    bool operator()(const std::tuple<char, size_t, char>& lhs,
                    const std::tuple<char, size_t, char>& rhs) const {

        return std::get<1>(lhs) < std::get<1>(rhs);
    }
};


class SmithWaterman {

public:
    typedef std::set<std::tuple<char, size_t, char>, mutationTuple_comparator> MutationsTupleSet;

private:
    typedef struct {
        int score;
        int parent;
    } Cell;

private:
    std::string source_;
    std::string target_;

    bool use_blosum_;

    std::vector<std::vector<Cell>> matrix_;

    int max_score_;
    size_t max_score_i_;
    size_t max_score_j_;

public:
    SmithWaterman(const std::string& source,
                  const std::string& target,
                  bool use_blosum = true);

    void print_matrix(bool scoreQ = true);

    void reconstruct_path(size_t start_ref, MutationsTupleSet& mutations);
    void reconstruct_path(size_t start_ref, size_t i, size_t j, MutationsTupleSet& mutations);

private:
    void calcMatrix();

    int bolsum32match(char c, char d);

    int match(char c, char d);
    int indel(char c);
    int insert(char c);
};
