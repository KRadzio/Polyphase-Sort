#ifndef SORTER_HPP
#define SORTER_HPP

#include <string>
#include <vector>
#include <cmath>

#include <ncurses.h>

#include "FileManager.hpp"
#include "Tape.hpp"

class Sorter
{
public:
    static Sorter &GetInstance();
    void Sort(std::string inputTapeName, bool skip = false);

    void SetWindow(_win_st *window);

    inline size_t GetNumberOfPhases() { return numberOfPhases; }
    inline size_t GetInitialNumberOfSeries() { return initialSerieCount; }
    inline size_t GetNumberOfRecords() { return recordsCount; }
    inline size_t GetNumberOfFileAccesses() { return fileAccesses; }
    inline size_t GetNumberOfReads() { return reads; }
    inline size_t GetNumberOfWrites() { return writes; }
    inline float GetTheoreticalNumberoOfPhases() { return phasesT; }
    inline float GetTheoreticalFileAccesses() { return fileAccesesT; }

private:
    size_t Fib(int n);
    void SplitToTapes(std::string inputTapeName, bool skip);
    void FillTapeUpToCurrGoal(Tape &currTape, Tape &otherTape);
    void SetUpTapesBeforeSorting();
    void SwapAndClearTapes();
    void FillWithShorterTapeSerie(std::string &recordS);
    void MergeTwoSeries(std::string &recordS, std::string &recordL);
    void InsertNextRecord(std::string &recordS, std::string &recordL);
    void ResetTapes();
    int FindClosestFibNumberIndex(size_t seriesCount);

private:
    Sorter();
    ~Sorter();

private:
    // buffers
    Tape tape1;
    Tape tape2;
    Tape tape3;
    // used in sorting
    Tape *shorterTape = nullptr;
    Tape *longerTape = nullptr;
    Tape *emptyTape = nullptr;
    // used in sorting
    size_t dummyCount = 0;
    size_t currFib;
    size_t fibIndex = 1;
    size_t numberOfPhases;
    // stats
    size_t recordsCount = 0;
    size_t initialSerieCount = 0;
    size_t reads = 0;
    size_t writes = 0;
    size_t fileAccesses = 0;
    float fileAccesesT;
    float phasesT;

    _win_st *window;
};
#endif
