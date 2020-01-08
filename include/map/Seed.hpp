#include <string>

class Seed {
private:
    unsigned int start_pos_read_;
    unsigned int start_pos_reference_;

    unsigned int end_pos_read_;
    unsigned int end_pos_reference_;

public:
    explicit Seed(unsigned int start_pos_read, unsigned int start_pos_reference, unsigned int k);
    // Seed(unsigned int start_pos_read, unsigned int start_pos_reference);

    void extendLeft(std::string& read, std::string& reference);
    void extendRight(std::string& read, std::string& reference);
    void extendBoth(std::string& read, std::string& reference);

    friend std::ostream& operator<<(std::ostream &strm, const Seed &seed);
};
