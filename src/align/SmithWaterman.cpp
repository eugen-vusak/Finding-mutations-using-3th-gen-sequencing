#include "align/SmithWaterman.hpp"

#include <iostream>
#include <iomanip>

#define MATCH           0
#define INSERT          1
#define DELETE          2
#define penalty        -2

SmithWaterman::SmithWaterman(const std::string& source,
                             const std::string& target,
                             bool use_blosum)
    : source_(" " + source),
      target_(" " + target),
      use_blosum_(use_blosum),
      matrix_(source_.size(),
              std::vector<Cell>(target_.size(), Cell{0, -1})),
      max_score_(-1),
      max_score_i_(-1),
      max_score_j_(-1)
{

    calcMatrix();
}

int SmithWaterman::bolsum32match(char a, char b) {
    int i, j;

    auto i_iter = blosum_keys_to_index.find(a);
    if (i_iter == blosum_keys_to_index.end()) {
        std::cout << "WARRNING: " << a << "not found, used wildcard instead" << std::endl;
        i = blosum_keys_to_index[WILDCARD];
    } else {
        i = i_iter->second;
    }

    auto j_iter = blosum_keys_to_index.find(b);
    if (j_iter == blosum_keys_to_index.end()) {
        std::cout << "WARRNING: " << b << "not found, used wildcard instead" << std::endl;
        j = blosum_keys_to_index[WILDCARD];
    } else {
        j = j_iter->second;
    }

    return blosum_values[i][j];
}

inline int SmithWaterman::match(char c, char d) {
    if (c == d)
        return 5;
    else
        return  -4;
}

inline int SmithWaterman::indel(char) {
    return -4;
}

inline int SmithWaterman::insert(char) {
    return  -4;
}


void SmithWaterman::calcMatrix() {
    int opt[3];			// scores of three operations

    size_t source_size = source_.size();
    size_t target_size = target_.size();
    for (size_t i = 1; i < source_size; i++) {
        for (size_t j = 1; j < target_size; j++) {

            if(use_blosum_ == true) {
                opt[MATCH] = matrix_[i - 1][j - 1].score + bolsum32match(source_[i],target_[j]);
                opt[INSERT] = matrix_[i][j - 1].score + penalty;
                opt[DELETE] = matrix_[i - 1][j].score + penalty;
            } else {
                opt[MATCH] = matrix_[i - 1][j - 1].score + match(source_[i],target_[j]);
                opt[INSERT] = matrix_[i][j - 1].score + insert(target_[j]);
                opt[DELETE] = matrix_[i - 1][j].score + indel(source_[i]);
            }

            matrix_[i][j].parent = -1;
            for (int op = MATCH; op <= DELETE; op++) {
                if (opt[op] > matrix_[i][j].score) {
                    matrix_[i][j].score = opt[op];
                    matrix_[i][j].parent = op;
                }
            }

            // update best score in aligment matrix and its position, needed for path reconstruction later
            if(matrix_[i][j].score > max_score_) {
                max_score_ = matrix_[i][j].score;
                max_score_i_ = i;
                max_score_j_ = j;
            }
        }
    }
}

void SmithWaterman::reconstruct_path(size_t start_ref, MutationsTupleVector& mutations) {
    SmithWaterman::reconstruct_path(start_ref, max_score_i_, max_score_j_, mutations);
}

void SmithWaterman::reconstruct_path(size_t start_ref, size_t i, size_t j, MutationsTupleVector& mutations) {

    if (matrix_[i][j].parent == -1) {
        return;
    }

    if (matrix_[i][j].parent == MATCH) {
        reconstruct_path(start_ref, i - 1, j - 1, mutations);

        if(source_[i] != target_[j]) {
            mutations.push_back(std::make_tuple('X', start_ref + j, source_[i]));
        } else {
            // MATCH
        }
        return;
    }

    if (matrix_[i][j].parent == INSERT) {
        reconstruct_path(start_ref, i, j - 1, mutations);

        mutations.push_back(std::make_tuple('D', start_ref + j, '-'));
        return;
    }

    if (matrix_[i][j].parent == DELETE) {
        reconstruct_path(start_ref, i - 1, j, mutations);

        mutations.push_back(std::make_tuple('I', (start_ref + j - 1), source_[i]));
        return;
    }

}

void SmithWaterman::print_matrix(bool scoreQ) {

    std::cout << "   ";

    size_t source_size = source_.size();
    size_t target_size = target_.size();

    // print column names (target)
    for (size_t j = 0; j < target_size; j++) {
        std::cout << "  " << target_[j];
    }
    std::cout << std::endl;

    for (size_t i = 0; i < source_size; i++) {
        // print row names (source)
        std::cout << source_[i] << ": ";

        // print values
        for (size_t j = 0; j < target_size; j++) {
            if (scoreQ == true) {
                std::cout << " " << std::setw(2) << matrix_[i][j].score;
            } else {
                std::cout << " " << std::setw(2) << matrix_[i][j].parent;
            }
        }
        std::cout << std::endl;
    }
}
