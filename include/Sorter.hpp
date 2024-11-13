#ifndef SORTER_HPP
#define SORTER_HPP

#include <string>
#include <vector>

#include "FileManager.hpp"

class Sorter
{
public:
    static Sorter &GetInstance();
    void Sort(std::string inputTapeName);

private:
    size_t Fib(int n);
    std::string PickRecord(bool serieEnded);
    void SplitToTapes(std::string inputTapeName);
    void DefaultSplit(std::string inputTapeName, size_t prev2);
    void AlternativeSplit(std::string inputTapeName, size_t prev2);
    int FindClosestFibNumberIndex(size_t seriesCount);
    size_t CalculateNumberOfSeries(std::string tapeName);

private:
    Sorter();
    ~Sorter();

private:
    std::vector<std::string> inputOutputBuffer;
    std::vector<std::string> longerTapeBuffer;
    std::vector<std::string> shorterTapeBuffer;
    std::vector<std::string> longerTapeSeriesEnd;
    std::vector<std::string> shorterTapeSeriesEnd;
    //
    size_t dummyCount = 0;
    size_t seriesCount;
    size_t prev;
    size_t prev2;
    size_t index;
    size_t numberOfPhases;
};
#endif
