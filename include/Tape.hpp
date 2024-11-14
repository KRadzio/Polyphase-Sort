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

    std::string GetNextRecord();
    std::string GetSerieNextEnd();

    inline void incrementNumberOfSeries() { numberOfSeries++; }

private:
    std::vector<std::string> vectorOfRecords;
    std::vector<std::string> seriesEnd; // used in first phase
    size_t index = 0;
    size_t seriesEndIndex = 0;
    size_t numberOfSeries = 0; // may not be needed
    size_t blockNum = 1;
    std::string fileName;
    std::string lastRecordFromPreviousBlock;
};

#endif
