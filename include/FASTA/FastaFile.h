#include <string>
#include <fstream>

class FastaFile {
private:
    std::string filename_;
    std::ifstream file_stream_;
    bool opened_;

public:
    explicit FastaFile(const std::string& filename);
    ~FastaFile();

    bool open();
    void close();

};

