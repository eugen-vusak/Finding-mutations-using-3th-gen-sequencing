/**
 * @file blosum.hpp
 * @brief This file contains values from BLOSUM matrix
 *
 * BLOSUM (BLOcks SUbstitution Matrix) matrix is a substitution matrix
 * used for sequence alignment of proteins.
 * BLOSUM matrices are used to score alignments between
 * evolutionarily divergent protein sequences.
 * They are based on local alignments.
 */

#include <unordered_map>

#define BLOSUM_SIZE 24
#define WILDCARD    '*'

extern char blosum_keys[BLOSUM_SIZE];
extern int blosum_values[BLOSUM_SIZE][BLOSUM_SIZE];
extern std::unordered_map<char, int> blosum_keys_to_index;
