#include "Sorter.hpp"

Sorter::Sorter()
{
    inputOutputBuffer = std::vector<std::string>(BLOC_SIZE / RECORD_SIZE);
    longerTapeBuffer = std::vector<std::string>(BLOC_SIZE / RECORD_SIZE);
    shorterTapeBuffer = std::vector<std::string>(BLOC_SIZE / RECORD_SIZE);
}
Sorter::~Sorter() {}

Sorter &Sorter::GetInstance()
{
    static Sorter *instance = new Sorter;
    return *instance;
}

std::string Sorter::PickRecord(bool serieEnded) { return EMPTY_RECORD; }

void Sorter::Sort(std::string inputTapeName)
{
    seriesCount = CalculateNumberOfSeries(inputTapeName);
    index = FindClosestFibNumberIndex(seriesCount);
    numberOfPhases = index - 2;
    prev = Fib(index - 1);
    prev2 = Fib(index - 2);
    if (prev + prev2 != seriesCount && seriesCount != 1)
        dummyCount = prev + prev2 - seriesCount;
    SplitToTapes(inputTapeName); // dystribution

    // sorting

    FileManager::GetInstance().ClearFile(TAPE1);

    // in each faze one tape will become empty
    // prev will become prev2
    // prev2 will become Fib(n-1)
    std::string emptyTape = TAPE1;
    std::string longerTape = TAPE2;
    std::string shorterTape = TAPE3;

    size_t outputBufferIndex = 0;
    size_t longerTapeIndex = 0;
    size_t shorterTapeIndex = 0;
    size_t longerTapeBlockNum = 1;
    size_t shorterTapeBlockNum = 1;
    size_t longerTapeSerieIndex = 0;
    size_t shorterTapeSerieIndex = 0;

    // sorting
    size_t phase = 0;

    while (phase < numberOfPhases)
    {
        FileManager::GetInstance().ReadBlockFromFile(longerTape, longerTapeBlockNum, longerTapeBuffer);
        FileManager::GetInstance().ReadBlockFromFile(shorterTape, shorterTapeBlockNum, shorterTapeBuffer);
        size_t serieNum = 0; // the amount of series that will be merged in each phase, it is also the amount of phases on the shorter tape
        while (serieNum < prev2)
        {
            bool longerTapeSerieEnded = false;
            bool shorterTapeSerieEnded = false;
            std::string record1;
            std::string record2;

            while (!longerTapeSerieEnded || !shorterTapeSerieEnded)
            {
                if (!longerTapeSerieEnded)
                {
                    if (phase == 0)
                    {
                        // meaby check for dupes?
                        if (longerTapeBuffer[longerTapeIndex] == longerTapeSeriesEnd[longerTapeSerieIndex])
                        {
                            longerTapeSerieEnded = true;
                            record1 = longerTapeBuffer[longerTapeIndex];
                        }
                        else
                            record1 = longerTapeBuffer[longerTapeIndex];
                    }
                    else
                    {
                        if (longerTapeBuffer[longerTapeIndex + 1] < longerTapeBuffer[longerTapeIndex])
                        {
                            longerTapeSerieEnded = true;
                            record1 = longerTapeBuffer[longerTapeIndex];
                        }
                        else
                            record1 = longerTapeBuffer[longerTapeIndex];
                    }
                    longerTapeIndex++;
                }
                else
                    record1 = EMPTY_RECORD;

                if (!shorterTapeSerieEnded)
                {
                    if (phase == 0)
                    {
                        if (shorterTapeBuffer[shorterTapeIndex] == shorterTapeSeriesEnd[shorterTapeSerieIndex])
                        {
                            shorterTapeSerieEnded = true;
                            record2 = shorterTapeBuffer[shorterTapeIndex];
                        }
                        else
                            record2 = shorterTapeBuffer[shorterTapeIndex];
                    }
                    else
                    {
                        if (shorterTapeBuffer[shorterTapeIndex + 1] < shorterTapeBuffer[shorterTapeIndex])
                        {
                            shorterTapeSerieEnded = true;
                            record2 = shorterTapeBuffer[shorterTapeIndex];
                        }
                        else
                            record2 = shorterTapeBuffer[shorterTapeIndex];
                    }
                    shorterTapeIndex++;
                }
                else
                    record2 = EMPTY_RECORD;


                // record should be sorted inside serie
                // maybe use a buffer to store a serie ?
                if (record1 < record2)
                {
                    if (record1 != EMPTY_RECORD)
                    {
                        inputOutputBuffer[outputBufferIndex] = record1;
                        outputBufferIndex++;
                    }
                    if (record2 != EMPTY_RECORD)
                    {
                        inputOutputBuffer[outputBufferIndex] = record2;
                        outputBufferIndex++;
                    }
                }
                else
                {
                    if (record2 != EMPTY_RECORD)
                    {
                        inputOutputBuffer[outputBufferIndex] = record2;
                        outputBufferIndex++;
                    }
                    if (record1 != EMPTY_RECORD)
                    {
                        inputOutputBuffer[outputBufferIndex] = record1;
                        outputBufferIndex++;
                    }
                }
                std::cout << record1 << " " << record2 << std::endl;
            }

            if (longerTapeIndex == BLOC_SIZE / RECORD_SIZE)
            {
                longerTapeIndex = 0;
                longerTapeBlockNum++;
                FileManager::GetInstance().ReadBlockFromFile(longerTape, longerTapeBlockNum, longerTapeBuffer);
            }
            if (shorterTapeIndex == BLOC_SIZE / RECORD_SIZE)
            {
                shorterTapeIndex = 0;
                shorterTapeBlockNum++;
                FileManager::GetInstance().ReadBlockFromFile(shorterTape, shorterTapeBlockNum, shorterTapeBuffer);
            }
            if (outputBufferIndex == BLOC_SIZE / RECORD_SIZE)
            {
                FileManager::GetInstance().WriteBlockToFile(emptyTape, inputOutputBuffer);
                outputBufferIndex = 0;
            }
            // pick one record from each serie and put then into the output buffer
            // if one record is smaller than the last inserted then we should reorganize the buffer
            // if the buffer is full then save it to the "empty tape"
            // the two other buffer also have to be refreshed after we reached the end of them
            // if a serie ended then we record that we picked is empty

            if (phase == 0)
            {
                longerTapeSerieIndex++;
                shorterTapeSerieIndex++;
            }
            serieNum++;
        }
        // after the first loop we do not need the serieindex becouse no more series will combine
        // if a serie is made for example of "f" , "f" , "f" and the previous serie ended with "f" the remembering end of series may not work properly
        // save the output buffer after all series in a phase are merged

        FileManager::GetInstance().WriteBlockToFile(emptyTape, inputOutputBuffer);

        index--;
        prev = Fib(index - 1);
        prev2 = Fib(index - 2);

        FileManager::GetInstance().ClearFile(shorterTape);

        shorterTapeIndex = longerTapeIndex;
        shorterTapeBlockNum = longerTapeBlockNum;
        longerTapeIndex = 0;
        longerTapeBlockNum = 1;
        outputBufferIndex = 0;

        std::string tmp = emptyTape;
        emptyTape = shorterTape;
        shorterTape = longerTape;
        longerTape = tmp;
        phase++;
    }
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

    longerTapeSeriesEnd = std::vector<std::string>(prev); // read after merging with dummies
    shorterTapeSeriesEnd = std::vector<std::string>(prev2);

    // by default "odd" series go to tape1, "even" series go to tape2
    // serie 1,3,5,...2n+1 -> tape1 (prev2 series)
    // serie 0,2,4,...2n   -> tape2 (prev series) (dummy series go here)
    // if seriesCount / 2 is smaller than prev2 we should split the input tape in a diffrent way
    // remember about coalescing
    if (seriesCount / 2 < prev2) // other form of distribution
        AlternativeSplit(inputTapeName, prev2);
    else // even - odd distribution
        DefaultSplit(inputTapeName, prev2);

    FileManager::GetInstance().ClearBufferFromIndex(inputOutputBuffer, 0);
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

void Sorter::AlternativeSplit(std::string inputTapeName, size_t prev2)
{
    size_t blockIndex = 1;
    size_t shorterTapesSeriesCount = 0;
    size_t inputTapeIndex = 0;
    size_t longerTapeIndex = 0;
    size_t shorterTapeIndex = 0;
    size_t longerTapeSeriesIndex = 0;
    size_t shorterTapeSeriesIndex = 0;
    std::string lastRecordFromPrevBlock = "";
    while (FileManager::GetInstance().ReadBlockFromFile(inputTapeName, blockIndex, inputOutputBuffer))
    {
        while (inputTapeIndex < inputOutputBuffer.size() - 1 && inputOutputBuffer[inputTapeIndex] != "")
        {
            if (shorterTapesSeriesCount < prev2) // first fill the tape that will contain less series
            {
                if (inputTapeIndex == 0 && lastRecordFromPrevBlock > inputOutputBuffer[inputTapeIndex]) // check if serie ended in previous block

                {

                    shorterTapeSeriesEnd[shorterTapeSeriesIndex] = inputOutputBuffer[inputTapeIndex];
                    shorterTapeSeriesIndex++;
                    shorterTapesSeriesCount++;
                }
                if (inputOutputBuffer[inputTapeIndex] > inputOutputBuffer[inputTapeIndex + 1] || inputOutputBuffer[inputTapeIndex + 1] == EMPTY_RECORD)
                {
                    shorterTapeBuffer[shorterTapeIndex] = inputOutputBuffer[inputTapeIndex];
                    shorterTapeSeriesEnd[shorterTapeSeriesIndex] = inputOutputBuffer[inputTapeIndex];
                    shorterTapeSeriesIndex++;
                    shorterTapeIndex++;
                    shorterTapesSeriesCount++;
                }
                else
                {
                    shorterTapeBuffer[shorterTapeIndex] = inputOutputBuffer[inputTapeIndex];
                    shorterTapeIndex++;
                }
                if (inputTapeIndex == inputOutputBuffer.size() - 2) // the next record is unnown so we continue or start a new serie (the turn is set)
                {
                    lastRecordFromPrevBlock = inputOutputBuffer[inputTapeIndex + 1];
                    shorterTapeBuffer[shorterTapeIndex] = inputOutputBuffer[inputTapeIndex + 1];
                    shorterTapeIndex++;
                }
            }
            else // write the rest to the other tape (it will contain dummy series)
            {
                if (inputTapeIndex == 0 && lastRecordFromPrevBlock > inputOutputBuffer[inputTapeIndex]) // check if serie ended in previous block

                {
                    longerTapeSeriesEnd[longerTapeSeriesIndex] = inputOutputBuffer[inputTapeIndex];
                    longerTapeSeriesIndex++;
                    shorterTapesSeriesCount++;
                }
                if (inputOutputBuffer[inputTapeIndex] > inputOutputBuffer[inputTapeIndex + 1] || inputOutputBuffer[inputTapeIndex + 1] == EMPTY_RECORD)
                {
                    longerTapeSeriesEnd[longerTapeSeriesIndex] = inputOutputBuffer[inputTapeIndex];
                    longerTapeSeriesIndex++;
                }
                longerTapeBuffer[longerTapeIndex] = inputOutputBuffer[inputTapeIndex];
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

size_t Sorter::CalculateNumberOfSeries(std::string tapeName)
{
    size_t blockIndex = 1;
    size_t seriesCount = 0;
    std::string lastRecordFromPrevBlock = "";
    while (FileManager::GetInstance().ReadBlockFromFile(tapeName, blockIndex, inputOutputBuffer))
    {
        size_t index = 0;
        while (index < inputOutputBuffer.size() - 1 && inputOutputBuffer[index] != "")
        {
            if (index == 0 && lastRecordFromPrevBlock > inputOutputBuffer[index]) // Serie could end at the last record of the previous block
                seriesCount++;
            if (inputOutputBuffer[index] > inputOutputBuffer[index + 1] || inputOutputBuffer[index + 1] == "")
                seriesCount++;
            index++;
            if (index == inputOutputBuffer.size() - 2)
                lastRecordFromPrevBlock = inputOutputBuffer[index + 1];
        }
        blockIndex++;
    }
    return seriesCount;
}
