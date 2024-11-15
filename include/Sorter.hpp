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
    void DefaultSplit(std::string inputTapeName, size_t prev2);
    int FindClosestFibNumberIndex(size_t seriesCount);

private:
    Sorter();
    ~Sorter();

private:
    Tape tape1;
    Tape tape2;
    Tape tape3;
    std::vector<std::string> inputOutputBuffer;
    std::vector<std::string> longerTapeBuffer;
    std::vector<std::string> shorterTapeBuffer;
    std::vector<std::string> longerTapeSeriesEnd;
    std::vector<std::string> shorterTapeSeriesEnd;
    //
    size_t dummyCount = 0;
    size_t seriesCount;
    size_t currFib;
    size_t prev = 1;
    size_t prev2 = 1;
    size_t index = 1;
    size_t numberOfPhases;
};
#endif
