#include <vector>
#include <string>
#include <map>
#include <unordered_set>

/**
 * @brief Class that describes one record in Fasta file
 *
 * Fasta file Record. Contains header marked by > in fasta file
 * and sequence of data.
 *
 */
class FastaRecord {

public:
    typedef std::map<std::string, std::unordered_set<uint>> MinimizersTable;

public:
    std::string sequence_;
    std::string header_;

public:
    FastaRecord();
    FastaRecord(const FastaRecord &) = default;

    /**
     * @brief Set the header of record
     *
     * @param header - string value of header
     */
    void setHeader(const std::string& header);

    /**
     * @brief extends records sequence by appending provided sequence to existing
     *
     * @param sequence - sequence to be appended to record's sequence
     *
     */
    void extendSequence(const std::string& sequence);


    /**
     * @brief Computes and returns vector of k-mer minimizers from record's attribute
     * sequence_.
     *
     * Computes and returns vector of k-mer minimizers from record's attribute sequence_.
     * Minimizers are uniquely chosen representative k-mers from a group of adjacent k-mers. Thus,
     * each minimizer represents one sequence's window. By using minimizers, memory usage can be
     * reduced significantly.
     *
     * Having w <= k guarantees that no gaps appear between adjacent minimizers.
     *
     * @param k size of a k-mer
     * @param w number of consecutive adjacent k-mers in a window.
     * @return map of minimizers to positions where they occur
     *
     * @see https://academic.oup.com/bioinformatics/article/20/18/3363/202143
     */
    MinimizersTable getMinimizers(short k, short w);

    FastaRecord& operator=(const FastaRecord &) = default;
    friend std::ostream& operator<<(std::ostream &strm, const FastaRecord &record);
};
