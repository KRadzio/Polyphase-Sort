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
    void InsertNewRecord(bool &first, Tape *tape, bool &serieEnded, std::string& serieEnd, bool longer = false); // bool references aren't the best thing
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
