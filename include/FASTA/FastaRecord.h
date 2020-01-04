#include <vector>
#include <string>

class FastaRecord {

public:
    std::string sequence_;
    std::string header_;

public:
    FastaRecord();
    FastaRecord(const FastaRecord &) = default;

    void setHeader(const std::string& header);
    void extendSequence(const std::string& sequence);

    std::vector<std::string> getMinimizers(short k, short w);

    FastaRecord& operator=(const FastaRecord &) = default;
    friend std::ostream& operator<<(std::ostream &strm, const FastaRecord &record);
};
