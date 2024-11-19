#ifndef TAPE_HPP
#define TAPE_HPP

#include <string>
#include <vector>

#include "FileManager.hpp"

// this one needs cleanup
class Tape
{

public:
    Tape();
    ~Tape();

    std::string GetNextRecord();
    void SetNextRecord(std::string newRecord);

    // if we merge two series the new record may not be in order
    // example: we have "a" and "c" in the buffer and we want to add "b"
    // we have to reorganize the serie in buffer
    void SetNextRecordAndSortSerie(std::string newRecord);
    std::string GetSerieEnd();
    void SetNextSerieEnd(std::string newSerieEnd);

    void ResetIndex(bool save = true); // used when tape has ended or its purpuse is swaped from read to write
    void Save();
    void SetFileAndFillBuffer(std::string filename); // change file, reset index and load first block from new file

    inline void ClearBuffer() { FileManager::GetInstance().ClearBufferFromIndex(vectorOfRecords, 0); }
    inline void FillBuffer() { FileManager::GetInstance().ReadBlockFromFile(filename, blockNum, vectorOfRecords); }


    inline void ReduceNumberOfSerie(size_t amount) {numberOfSeries -= amount;}
    inline void ResetSeriesEnd() {seriesEnd.clear();}
    inline void Clear(){FileManager::GetInstance().ClearFile(filename);} // to clear a file
    inline void SetFile(std::string filename) { this->filename = filename; }
    inline bool HasEnded() { return hasEnded; }
    inline std::string GetPrevRecord() { return prevRecord; }
    inline void IncrementNumberOfSeries() { numberOfSeries++; }
    inline size_t GetNumberOfSeries() { return numberOfSeries; }

private:
    void checkPrevBlocks(std::string newRecord, size_t currPos);

private:
    std::vector<std::string> vectorOfRecords;
    std::vector<std::string> swapBuffer; // used when serie is longer than one block or a record needs to be placed in block than was already saved
    std::vector<std::string> tmpV; // used when we reach index 0 in swapBuffer
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
