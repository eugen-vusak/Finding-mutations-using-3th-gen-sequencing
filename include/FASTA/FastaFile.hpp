#ifndef FASTA_FILE_HPP
#define FASTA_FILE_HPP

#include <string>
#include <fstream>

#include <FASTA/FastaRecord.hpp>

/**
 * @brief class that describes file of FASTA format
 *
 * This is wrapper around std::ifstream for easier work with
 * .fasta files. It enables simple iterating through records
 * in opened file.
 *
 * @see http://www.bioinformatics.nl/tools/crab_fasta.html
 * @see http://www.cplusplus.com/reference/fstream/ifstream/
 */
class FastaFile {
private:
    std::string filename_;      // path to file
    std::ifstream file_stream_; // ifstream object used for reading file

    bool has_next_record_;      // flag marking if file has atleast one record for reading
    std::string next_header_;   // cache for header that will be in next record that is created

public:
    /**
     * @brief Construct a new FastaFile object and opens it
     *
     * @param filename - path to Fasta file
     */
    explicit FastaFile(const std::string& filename);

    /**
     * @brief Destroy the Fasta File object
     *
     * This function will destroy fasta file object, if
     * file was opened it will close it
     */
    ~FastaFile();

    /**
     * @brief Opens file
     *
     * @return true if file was opened succesfully
     * @return false else
     */
    bool open();

    /**
     * @brief closes file
     */
    void close();

    /**
     * @brief returns if file has next record for reading
     *
     * @return true if next record exists
     * @return false if next record does not exist
     */
    bool hasNextRecord();

    /**
     * @brief Get the Next Record object
     *
     * @return next FastaRecord if record exists
     * @throw out_of_range exception if no more records
     */
    FastaRecord getNextRecord();

private:

    /**
     * @brief reads first next header in file
     *
     * Reads first next header in file. All lines before it are ignored
     *
     * @note This function should only be called once when file is opened to read
     * first records header. If this function is called when file position is on
     * some record that record will be skipped over
     */
    void readHeader();

};

#endif // !FASTA_FILE_HPP
