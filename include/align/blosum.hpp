#include <unordered_map>

#define BLOSUM_SIZE 24
#define WILDCARD    '*'

extern char blosum_keys[BLOSUM_SIZE];
extern int blosum_values[BLOSUM_SIZE][BLOSUM_SIZE];
extern std::unordered_map<char, int> blosum_keys_to_index;
