#include <string>
#include <fstream>

#include <FASTA/FastaRecord.h>

class FastaFile {
private:
    std::string filename_;
    std::ifstream file_stream_;

    bool has_next_record_;
    std::string next_header_;

public:
    explicit FastaFile(const std::string& filename);
    ~FastaFile();

    bool open();
    void close();

    bool hasNextRecord();
    FastaRecord getNextRecord();

private:
    void readHeader();

};

