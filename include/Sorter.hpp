#ifndef SORTER_HPP
#define SORTER_HPP

#include <string>
#include <vector>

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
    int FindClosestFibNumberIndex(size_t seriesCount);

private:
    Sorter();
    ~Sorter();

private:
    Tape tape1;
    Tape tape2;
    Tape tape3;
    //
    size_t dummyCount = 0;
    size_t currFib;
    size_t fibIndex = 1;
    size_t numberOfPhases;
};
#endif
