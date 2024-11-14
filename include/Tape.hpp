#ifndef TAPE_HPP
#define TAPE_HPP

#include <string>
#include <vector>

#include "FileManager.hpp"

class Tape
{

public:
    Tape();
    Tape(std::string filename);
    ~Tape();
private:
    std::vector<std::string> vectorOfRecord;
    std::vector<std::string> seriesEnd; // used in first phase
    size_t index = 0;
    size_t blockNum = 1;
    std::string fileName;
    std::string lastRecordFromPreviousBlock;
};

#endif
