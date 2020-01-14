#ifndef SMITHWATERMAN_HPP
#define SMITHWATERMAN_HPP

#include "align/blosum.hpp"

#include <string>
#include <vector>
#include <tuple>
#include <set>

/**
 * @brief class that provides Smith-Waterman alignment algorithm along with
 * additional utilities like printing similarity matrix and finding mutations
 * from computed sequence alignment.
 *
 * @see https://www.fer.unizg.hr/_download/repository/bioinformatika_skripta_v1.2.pdf
 */
class SmithWaterman {

public:
    typedef std::vector<std::tuple<char, size_t, char>> MutationsTupleVector;

private:
    /**
     * @brief struct that represents a cell in similarity matrix
     */
    typedef struct {
        int score;
        int parent;
    } Cell;

private:
    std::string source_;        // stores source sequence string
    std::string target_;        // stores target sequence string

    uint32_t band_width_;

    // determines if blosum matrix is used in computing similarity matrix
    bool use_blosum_;

    // similarity matrix
    std::vector<std::vector<Cell>> matrix_;

    // maximum score in the matrix
    int max_score_;
    // position of max score cell in the matrix
    size_t max_score_i_;
    size_t max_score_j_;

public:
    SmithWaterman() = default;

    SmithWaterman(const std::string& source,
                  const std::string& target,
                  bool use_blosum = true);

    /**
     * @brief Construct a new Banded Smith Waterman object
     *
     * This version of Smith Waterman algorithm only computes values inside a band
     * that is band_width away from either of diagonals of matrix
     *
     * @param source source sequence
     * @param target target sequence
     * @param band_width width of band from both diagonales
     * @param use_blosum flag telling if class will use blosum matrix
     */
    SmithWaterman(const std::string& source,
                  const std::string& target,
                  uint32_t band_width,
                  bool use_blosum = true);

    /**
     * @brief prints similarity matrix, used for printing score or parent values
     *
     * @param scoreQ is true for scores and false for parents
     */
    void print_matrix(bool scoreQ = true);

    /**
     * @brief reconstructs alignment path between object's source and target strings.
     *
     * Reconstruction starts from the cell with highest score in similiarity matrix and
     * continues with tracing back direction of creation of that cell.
     * Mutations are saved in mutation vector.
     *
     * @param start_ref starting index of reference sequence
     * @param mutations vector where mutations are saved
     */
    void reconstruct_path(size_t start_ref, MutationsTupleVector& mutations);

    /**
     * @brief reconstructs alignment path between object's source and target string.
     *
     * Reconstruction starts from the position (i, j) in similiarity matrix and
     * and continues with tracing back direction of creation of that cell.
     * Mutations are saved in mutation vector.
     *
     * @param start_ref starting index of reference sequence
     * @param i row index of goal cell
     * @param j column index of goal cell
     * @param mutations vector where mutations are saved
     */
    void reconstruct_path(size_t start_ref, size_t i, size_t j, MutationsTupleVector& mutations);

private:
    inline void updateCell(int (&opt)[3], uint32_t i, uint32_t j);
    
    /**
     * @brief Compute two-dimensional similarity matrix
     * 
     * Builds two-dimensional similarity matrix by computing score and parent value of every cell in
     * the matrix, starting from top left to bottom right cell, row by row. Score value is set by choosing
     * maximum value between three top adjacent cells, except if all three are negative, then zero is set
     * instead. Direction of calculated cell is saved in parent variable which is used later in path
     * reconstruction. Simultaneously, while computing simlarity matrix highest cell score in matrix is
     * traced and saved.
     */
    void calcMatrix();
    void calcMatrixBandSourceLarger();
    void calcMatrixBandTargetLarger();

    /**
     * @brief used for comparing two characters in blosum matrix
     *
     * @param c char
     * @param d char
     * @return match value of given characters
     */
    int bolsum32match(char c, char d);

    /**
     * @brief returns match or mismatch value for given characters
     *
     * @param c cha
     * @param d char
     * @return match or mismatch value
     */
    int match(char c, char d);

    /**
     * @brief returns indel value
     *
     * @return indel value
     */
    int indel();

    /**
     * @brief returns insertion value
     *
     * @return insertion value
     */
    int insert();
};

#endif // !SMITHWATERMAN_HPP
