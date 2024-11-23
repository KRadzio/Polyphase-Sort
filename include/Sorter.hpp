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

    inline size_t GetNumberOfPhases() { return numberOfPhases; }
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
    // used in distribution
    size_t recordsCount = 0;
    float fileAccesesT;
    float phasesT;
};
#endif
