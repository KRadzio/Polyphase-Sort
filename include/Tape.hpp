#ifndef TAPE_HPP
#define TAPE_HPP

#include <string>
#include <vector>

#include <ncurses.h>

#include "FileManager.hpp"

class Tape
{

public:
    Tape();
    ~Tape();

    // getting and setting records
    std::string GetNextRecord();
    inline std::string GetPrevRecord() { return prevRecord; }
    void SetNextRecord(std::string newRecord);

    // runs end operations
    std::string GetSerieEnd();
    void SetNextSerieEnd(std::string newSerieEnd);
    inline void ResetSeriesEnd() { seriesEnd.clear(); }

    // reseting tapes after phase
    void ResetIndex(bool save = true); // used when tape has ended or its purpuse is swaped from read to write
    void Save(); // save buffer (used after sorting is done to save the last block that may not be full)
    void SetFileAndFillBuffer(std::string filename); // change file, reset index and load first block from new file
    void ClearBuffer();

    void Diplay();

    // file operation
    inline void Clear() { FileManager::GetInstance().ClearFile(filename); } // to clear a file
    inline void SetFile(std::string filename) { this->filename = filename; }

    // runs count
    inline void ResetRunsCount() { numberOfInitialRuns = 0; }
    inline void IncrementNumberOfRuns() { numberOfInitialRuns++; }
    inline size_t GetNumberOfRuns() { return numberOfInitialRuns; } // returns INITIAL number of runs (not used after distribution phase)

    inline bool HasEnded() { return hasEnded; }

    inline void SetWindow(_win_st* window){this->window = window;}

private:
    std::vector<std::string> vectorOfRecords;
    std::vector<std::string> seriesEnd;
    size_t index = 0;
    size_t runsEndIndex = 0;
    size_t numberOfInitialRuns = 0;
    size_t blockNum = 1;
    std::string filename;
    std::string prevRecord = EMPTY_RECORD;
    bool hasEnded = false;
    _win_st * window;
};

#endif
