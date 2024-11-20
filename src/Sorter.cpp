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

// here
void Sorter::Sort(std::string inputTapeName)
{
    SplitToTapes(inputTapeName); // dystribution

    // sorting

    SetUpTapesBeforeSorting();

    size_t currPhase = 0;
    std::string recordS = EMPTY_RECORD; // if we reached the end of shorter tape this will be reseted to EMPTY_RECORD
    std::string recordL = EMPTY_RECORD;

    // phase 6 something breaks
    while (currPhase < numberOfPhases)
    {
        size_t seriesCount = 0;

        while (seriesCount < Fib(numberOfPhases - currPhase))
        {
            // we first remove the dummy series
            if (dummyCount > 0)
            {
                dummyCount--;
                FillWithShorterTapeSerie(recordS);
            }
            else
                MergeTwoSeries(recordS, recordL);
            seriesCount++;
        }
        currPhase++;
        if (currPhase < numberOfPhases)
        {
             SwapAndClearTapes();
             recordS = recordL;
             recordL = EMPTY_RECORD;
        }
           
    }
    emptyTape->Save();
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

    currFib = Fib(fibIndex);
    int turn = 0;

    while (!tape1.HasEnded())
    {
        if (turn % 2 == 0)
        {
            FillTapeUpToCurrGoal(tape2, tape3);
            fibIndex++;
            currFib = Fib(fibIndex);
            turn++;
        }
        else
        {
            FillTapeUpToCurrGoal(tape3, tape2);
            fibIndex++;
            currFib = Fib(fibIndex);
            turn++;
        }
    }

    numberOfPhases = fibIndex - 2;
    if (currFib != tape2.GetNumberOfSeries() + tape3.GetNumberOfSeries())
        dummyCount = currFib - tape2.GetNumberOfSeries() - tape3.GetNumberOfSeries();

    // something is wrong here
    std::cout << "Theoretical number of file accesses " << 2 * 1000 * (1.04 * log2(tape2.GetNumberOfSeries() + tape3.GetNumberOfSeries()) + 1) / (BLOC_SIZE / RECORD_SIZE) << std::endl;

    std::cout << currFib << " " << fibIndex << " " << tape2.GetNumberOfSeries() << " " << tape3.GetNumberOfSeries() << " " << dummyCount << " " << numberOfPhases << std::endl;
}

void Sorter::FillTapeUpToCurrGoal(Tape &currTape, Tape &otherTape)
{
    while (currTape.GetNumberOfSeries() < currFib)
    {
        std::string record = tape1.GetNextRecord();
        std::string prevRecord = tape1.GetPrevRecord();
        if (record == EMPTY_RECORD)
        {
            currTape.SetNextSerieEnd(prevRecord);
            currTape.IncrementNumberOfSeries();
            break;
        }
        if (prevRecord > record)
        {
            currTape.SetNextSerieEnd(prevRecord);
            currTape.IncrementNumberOfSeries();
            // we will write the record to the other tape if we reached the current goal
            if (currTape.GetNumberOfSeries() == currFib)
                otherTape.SetNextRecord(record);
            else
                currTape.SetNextRecord(record);
        }
        else
            currTape.SetNextRecord(record);
    }
}

// here
void Sorter::SetUpTapesBeforeSorting()
{
    // clear tape one since it will be empty
    tape1.SetFile(TAPE1);
    tape1.ResetIndex(false);
    tape1.ClearBuffer();
    tape1.Clear();
    emptyTape = &tape1;

    // pick the shorter one
    if (tape2.GetNumberOfSeries() == Fib(fibIndex - 2))
    {
        shorterTape = &tape2;
        longerTape = &tape3;
    }
    if (tape3.GetNumberOfSeries() == Fib(fibIndex - 2))
    {
        shorterTape = &tape3;
        longerTape = &tape2;
    }

    // reset index
    shorterTape->ResetIndex();
    longerTape->ResetIndex();
}
// here
void Sorter::SwapAndClearTapes()
{
    // save since buffer is auto-saved only if full
    // emptyTape->Save();

    // swap tapes
    Tape *tmp = emptyTape;
    emptyTape = shorterTape;
    shorterTape = longerTape;
    longerTape = tmp;

    // Clear the file and reset index in tape
    emptyTape->Clear();
    emptyTape->ClearBuffer();
    emptyTape->ResetIndex(false);
    emptyTape->ResetSeriesEnd();

    // Reset index on the previously empty tape (swap from write to read)
    longerTape->ResetIndex();
}

void Sorter::FillWithShorterTapeSerie(std::string &recordS)
{
    bool serieEndedS = false;
    std::string serieEndS = EMPTY_RECORD;
    serieEndS = shorterTape->GetSerieEnd();
    emptyTape->SetNextSerieEnd(serieEndS);

    // new serie begin
    if (recordS != EMPTY_RECORD)
        emptyTape->SetNextRecord(recordS);
    while (!serieEndedS)
    {
        recordS = shorterTape->GetNextRecord();
        std::string prev = shorterTape->GetPrevRecord();
        if (prev == serieEndS && recordS != serieEndS)
            serieEndedS = true;
        else
            emptyTape->SetNextRecord(recordS);
    }
}
void Sorter::MergeTwoSeries(std::string &recordS, std::string &recordL)
{
    bool serieEndedS = false;
    bool serieEndedL = false;
    std::string serieEndS = shorterTape->GetSerieEnd();
    std::string serieEndL = longerTape->GetSerieEnd();

    if (serieEndS > serieEndL)
        emptyTape->SetNextSerieEnd(serieEndS);
    else
        emptyTape->SetNextSerieEnd(serieEndL);

    // new serie begin
    if (recordS != EMPTY_RECORD && recordL != EMPTY_RECORD)
        InsertNextRecord(recordS, recordL);
    else if (recordS != EMPTY_RECORD) // after dummy series
    {
        recordL = longerTape->GetNextRecord();
        InsertNextRecord(recordS, recordL);
    }
    else if (recordL != EMPTY_RECORD) // if shorter tape ended
    {
        recordS = shorterTape->GetNextRecord();
        InsertNextRecord(recordS, recordL);
    }

    while (!serieEndedS || !serieEndedL)
    {
        if (shorterTape->GetPrevRecord() == serieEndS && recordS != serieEndS)
            serieEndedS = true;
        if (longerTape->GetPrevRecord() == serieEndL && recordL != serieEndL)
            serieEndedL = true;

        if (!serieEndedL && !serieEndedS)
            InsertNextRecord(recordS, recordL);
        else if (!serieEndedL)
        {
            emptyTape->SetNextRecord(recordL);
            recordL = longerTape->GetNextRecord();
        }
        else if (!serieEndedS)
        {
            emptyTape->SetNextRecord(recordS);
            recordS = shorterTape->GetNextRecord();
        }         
    }
}

void Sorter::InsertNextRecord(std::string &recordS, std::string &recordL)
{
    if (recordL > recordS)
    {
        emptyTape->SetNextRecord(recordS);
        recordS = shorterTape->GetNextRecord();
    }
    else
    {
        emptyTape->SetNextRecord(recordL);
        recordL = longerTape->GetNextRecord();
    }
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
