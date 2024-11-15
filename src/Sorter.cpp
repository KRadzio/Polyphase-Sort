#include "Sorter.hpp"

Sorter::Sorter()
{
    tape1.SetFile(TAPE1);
    tape2.SetFile(TAPE2);
    tape3.SetFile(TAPE3);
}
Sorter::~Sorter()
{
    tape1.~Tape();
    tape2.~Tape();
    tape3.~Tape();
}

Sorter &Sorter::GetInstance()
{
    static Sorter *instance = new Sorter;
    return *instance;
}

void Sorter::Sort(std::string inputTapeName)
{
    SplitToTapes(inputTapeName); // dystribution

    // sorting

    FileManager::GetInstance().ClearFile(TAPE1);
}

size_t Sorter::Fib(int n)
{
    if (n < 1)
        return 0;
    if (n == 1 || n == 2)
        return 1;
    else
    {
        size_t prev = 1;
        size_t curr = 1;
        size_t tmp;
        int i = 2;
        while (i < n)
        {
            tmp = curr;
            curr += prev;
            prev = tmp;
            i++;
        }
        return curr;
    }
}

void Sorter::SplitToTapes(std::string inputTapeName)
{
    FileManager::GetInstance().ClearFile(TAPE2);
    FileManager::GetInstance().ClearFile(TAPE3);
    tape1.SetFileAndFillBuffer(inputTapeName);

    currFib = Fib(index);

    int turn = 0;

    while (!tape1.HasEnded())
    {
        if (turn % 2 == 0)
        {
            while (tape2.GetNumberOfSeries() < currFib)
            {
                std::string record = tape1.GetNextRecord();
                std::string prevRecord = tape1.GetPrevRecord();
                if (prevRecord > record)
                {
                    tape2.SetNextSerieEnd(prevRecord);
                    tape2.IncrementNumberOfSeries();
                    // we will write the record to the other tape if we reached the current goal
                    if (tape2.GetNumberOfSeries() == currFib && record != EMPTY_RECORD)
                        tape3.SetNextRecord(record);
                    else if (record != EMPTY_RECORD)
                        tape2.SetNextRecord(record);
                    // the loop won't break by itself
                    if (record == EMPTY_RECORD)
                        break;
                }
                else
                    tape2.SetNextRecord(record);
            }
            index++;
            currFib = Fib(index);
            turn++;
        }
        else
        {
            while (tape3.GetNumberOfSeries() < currFib)
            {
                std::string record = tape1.GetNextRecord();
                std::string prevRecord = tape1.GetPrevRecord();
                if (prevRecord > record)
                {
                    tape3.SetNextSerieEnd(prevRecord);
                    tape3.IncrementNumberOfSeries();
                    // we will write the record to the other tape if we reached the current goal
                    if (tape3.GetNumberOfSeries() == currFib && record != EMPTY_RECORD)
                        tape2.SetNextRecord(record);
                    else if (record != EMPTY_RECORD)
                        tape3.SetNextRecord(record);
                    // the loop won't break by itself
                    if (record == EMPTY_RECORD)
                        break;
                }
                else
                    tape3.SetNextRecord(record);
            }
            index++;
            currFib = Fib(index);
            turn++;
        }
    }
    tape2.Save();
    tape3.Save();

    if (turn % 2 == 1)
        std::cout << "Tape 2 is longer" << std::endl;
    else
        std::cout << "Tape 3 is longer" << std::endl;

    // and what if the input tape is sorted?
    if (tape2.GetNumberOfSeries() + tape3.GetNumberOfSeries() == 1)
        std::cout << "File already sorted" << std::endl;
    numberOfPhases = index - 2;
    if (currFib != tape2.GetNumberOfSeries() + tape3.GetNumberOfSeries())
        dummyCount = currFib - tape2.GetNumberOfSeries() - tape3.GetNumberOfSeries();

    std::cout << currFib << " " << index << " " << tape2.GetNumberOfSeries() << " " << tape3.GetNumberOfSeries() << " " << dummyCount << std::endl;
}

// split parts to smaller functions
void Sorter::DefaultSplit(std::string inputTapeName, size_t prev2)
{
    // move later to class
    size_t blockIndex = 1;
    size_t shorterTapesSeriesCount = 0;
    size_t inputTapeIndex = 0;
    size_t longerTapeIndex = 0;
    size_t shorterTapeIndex = 0;
    size_t longerTapeSeriesIndex = 0;
    size_t shorterTapeSeriesIndex = 0;
    std::string lastRecordFromPrevBlock = "";
    int turn = 0; // even at the start
    while (FileManager::GetInstance().ReadBlockFromFile(inputTapeName, blockIndex, inputOutputBuffer))
    {
        while (inputTapeIndex < inputOutputBuffer.size() - 1 && inputOutputBuffer[inputTapeIndex] != "")
        {
            if (shorterTapesSeriesCount < prev2) // write to two tapes
            {
                if (inputTapeIndex == 0 && lastRecordFromPrevBlock > inputOutputBuffer[inputTapeIndex]) // check if serie ended in previous block
                {
                    if (turn % 2 == 1)

                    {
                        shorterTapeSeriesEnd[shorterTapeSeriesIndex] = lastRecordFromPrevBlock;
                        shorterTapeSeriesIndex++;
                        shorterTapesSeriesCount++;
                    }
                    else
                    {
                        longerTapeSeriesEnd[longerTapeSeriesIndex] = lastRecordFromPrevBlock;
                        longerTapeSeriesIndex++;
                    }
                    turn++;
                }
                if (turn % 2 == 0) // tape 2
                {
                    if (inputOutputBuffer[inputTapeIndex] > inputOutputBuffer[inputTapeIndex + 1] || inputOutputBuffer[inputTapeIndex + 1] == EMPTY_RECORD)
                    {
                        longerTapeBuffer[longerTapeIndex] = inputOutputBuffer[inputTapeIndex];
                        longerTapeSeriesEnd[longerTapeSeriesIndex] = inputOutputBuffer[inputTapeIndex];
                        longerTapeSeriesIndex++;
                        longerTapeIndex++;
                        turn++;
                    }
                    else
                    {
                        longerTapeBuffer[longerTapeIndex] = inputOutputBuffer[inputTapeIndex];
                        longerTapeIndex++;
                    }
                }
                else // tape 3
                {
                    if (inputOutputBuffer[inputTapeIndex] > inputOutputBuffer[inputTapeIndex + 1] || inputOutputBuffer[inputTapeIndex + 1] == EMPTY_RECORD)
                    {
                        shorterTapeBuffer[shorterTapeIndex] = inputOutputBuffer[inputTapeIndex];
                        shorterTapeSeriesEnd[shorterTapeSeriesIndex] = inputOutputBuffer[inputTapeIndex];
                        shorterTapeSeriesIndex++;
                        shorterTapeIndex++;
                        shorterTapesSeriesCount++;
                        turn++;
                    }
                    else
                    {

                        shorterTapeBuffer[shorterTapeIndex] = inputOutputBuffer[inputTapeIndex];
                        shorterTapeIndex++;
                    }
                }
                if (inputTapeIndex == inputOutputBuffer.size() - 2) // the next record is unnown so we continue or start a new serie (the turn is set)
                {
                    lastRecordFromPrevBlock = inputOutputBuffer[inputTapeIndex + 1];
                    if (turn % 2 == 0)
                    {
                        longerTapeBuffer[longerTapeIndex] = inputOutputBuffer[inputTapeIndex + 1];
                        longerTapeIndex++;
                    }
                    else
                    {
                        shorterTapeBuffer[shorterTapeIndex] = inputOutputBuffer[inputTapeIndex + 1];
                        shorterTapeIndex++;
                    }
                }
            }
            else // write the rest to one tape
            {
                longerTapeBuffer[longerTapeIndex] = inputOutputBuffer[inputTapeIndex];
                if (inputOutputBuffer[inputTapeIndex] > inputOutputBuffer[inputTapeIndex + 1] || inputOutputBuffer[inputTapeIndex + 1] == EMPTY_RECORD)
                {
                    longerTapeSeriesEnd[longerTapeSeriesIndex] = inputOutputBuffer[inputTapeIndex];
                    longerTapeSeriesIndex++;
                }
                longerTapeIndex++;
                if (inputTapeIndex == inputOutputBuffer.size() - 2)
                {
                    longerTapeBuffer[longerTapeIndex] = inputOutputBuffer[inputTapeIndex + 1];
                    longerTapeIndex++;
                    lastRecordFromPrevBlock = inputOutputBuffer[inputTapeIndex + 1];
                }
            }
            inputTapeIndex++;
            // save if buffers are full
            if (longerTapeIndex == BLOC_SIZE / RECORD_SIZE)
            {
                longerTapeIndex = 0;
                FileManager::GetInstance().WriteBlockToFile(TAPE2, longerTapeBuffer);
            }
            if (shorterTapeIndex == BLOC_SIZE / RECORD_SIZE)
            {
                shorterTapeIndex = 0;
                FileManager::GetInstance().WriteBlockToFile(TAPE3, shorterTapeBuffer);
            }
        }
        blockIndex++;
        inputTapeIndex = 0;
    }
    // save since buffer may not be full
    FileManager::GetInstance().WriteBlockToFile(TAPE2, longerTapeBuffer);
    FileManager::GetInstance().WriteBlockToFile(TAPE3, shorterTapeBuffer);
}

int Sorter::FindClosestFibNumberIndex(size_t seriesCount)
{
    int index = 1;
    for (int i = 0; i < 1000; i++) // find first fib number that is grater or equal to seriesCount
    {
        if (Fib(i) >= seriesCount)
        {
            index = i;
            break;
        }
    }
    return index;
}