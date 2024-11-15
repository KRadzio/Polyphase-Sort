#ifndef TAPE_HPP
#define TAPE_HPP

#include <string>
#include <vector>

#include "FileManager.hpp"

class Tape
{

public:
    Tape();
    ~Tape();

    std::string GetNextRecord();
    void SetNextRecord(std::string newRecord);
    std::string GetSerieNextEnd();
    void SetNextSerieEnd(std::string newSerieEnd);

    void Save();
    void SetFileAndFillBuffer(std::string filename);

    inline void ClearBuffer() { FileManager::GetInstance().ClearBufferFromIndex(vectorOfRecords, 0); }
    inline void FillBuffer() { FileManager::GetInstance().ReadBlockFromFile(filename, blockNum, vectorOfRecords); }
    inline void SetFile(std::string filename) { this->filename = filename; }
    inline bool HasEnded() { return hasEnded; }
    inline std::string GetPrevRecord() { return prevRecord; }
    inline void IncrementNumberOfSeries() { numberOfSeries++; }
    inline size_t GetNumberOfSeries() { return numberOfSeries; }

private:
    std::vector<std::string> vectorOfRecords;
    std::vector<std::string> seriesEnd; // used in first phase of mergeing
    size_t index = 0;
    size_t seriesEndIndex = 0;
    size_t numberOfSeries = 0;
    size_t blockNum = 1;
    std::string filename;
    std::string prevRecord = EMPTY_RECORD;
    bool hasEnded = false;
};

#endif
