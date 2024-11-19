#ifndef SORTER_HPP
#define SORTER_HPP

#include <string>
#include <vector>
#include <cmath>

#include "FileManager.hpp"
#include "Tape.hpp"

class Sorter
{
public:
    static Sorter &GetInstance();
    void Sort(std::string inputTapeName);

private:
    size_t Fib(int n);
    void SplitToTapes(std::string inputTapeName);
    void FillTapeUpToCurrGoal(Tape &currTape, Tape &otherTape);
    void SetUpTapesBeforeSorting();
    void SwapAndClearTapes();
    void FillWithShorterTapeSerie(std::string& recordS);
    void MergeTwoSeries(std::string& recordS, std::string& recordL);
    void PutRecordsInOrder(std::string& recordS, std::string& recordL, bool firstTwoRecordsInsert = false);
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
};
#endif
