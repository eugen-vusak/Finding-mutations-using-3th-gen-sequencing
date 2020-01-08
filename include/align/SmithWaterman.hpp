#include "align/blosum.hpp"

#include <string>
#include <vector>

class SmithWaterman {

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

public:
    SmithWaterman(const std::string& source,
                  const std::string& target,
                  bool use_blosum = true);

    void print_matrix(bool scoreQ);
    void reconstruct_path(size_t i, size_t j);

private:
    void calcMatrix();

    int bolsum32match(char c, char d);

    int match(char c, char d);
    int indel(char c);
    int insert(char c);
};
