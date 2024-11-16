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

    // set tapes
    tape1.SetFile(TAPE1);
    tape1.ResetIndexAndBuffer();
    tape1.Clear();

    Tape *shorterTape = nullptr;
    Tape *longerTape = nullptr;
    Tape *emptyTape = &tape1;

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

    shorterTape->ResetIndexAndBuffer();
    shorterTape->FillBuffer();
    longerTape->ResetIndexAndBuffer();
    longerTape->FillBuffer();

    size_t currPhase = 0;

    while (currPhase < numberOfPhases)
    {
        size_t seriesCount = 0;
        size_t a = Fib(numberOfPhases - currPhase);
        while (seriesCount < Fib(numberOfPhases - currPhase))
        {
            bool serieEndedS = false;
            bool serieEndedL = false;

            std::string serieEndS = shorterTape->GetSerieEnd();
            std::string serieEndL = longerTape->GetSerieEnd();

            // choose new serie end
            if (serieEndS > serieEndL)
                emptyTape->SetNextSerieEnd(serieEndS);
            else
                emptyTape->SetNextSerieEnd(serieEndL);
            bool first = true;

            // check for dupes !!!!!!
            // there maybe some problems with merging if we run into dupes or when new blocks are loaded
            while (!serieEndedS || !serieEndedL)
            {
                if (!serieEndedS)
                {
                    std::string recordS = shorterTape->GetNextRecord();
                    // the record from the shorter tape will always be inserted first
                    // so it will always start a serie
                    if (first)
                    {
                        emptyTape->SetNextRecordAndSortSerie(recordS, true);
                        first = false;
                    }
                    else
                        emptyTape->SetNextRecordAndSortSerie(recordS);
                    if (recordS == serieEndS)
                        serieEndedS = true;
                }
                if (!serieEndedL)
                {
                    std::string recordL = longerTape->GetNextRecord();
                    emptyTape->SetNextRecordAndSortSerie(recordL);
                    if (recordL == serieEndL)
                        serieEndedL = true;
                }
            }
            seriesCount++;
        }
        // save since buffer is auto-saved only if full
        emptyTape->Save();

        // swap tapes
        Tape *tmp = emptyTape;
        emptyTape = shorterTape;
        shorterTape = longerTape;
        longerTape = tmp;

        // Clear the file and reset index in tape
        emptyTape->Clear();
        emptyTape->ResetIndexAndBuffer();
        emptyTape->ResetSeriesEnd();

        // Reset index on the previously empty tape (swap from write to read)
        longerTape->ResetIndexAndBuffer();
        longerTape->FillBuffer();

        currPhase++;
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
    // save since the buffer are not saved until they are full
    tape2.Save();
    tape3.Save();

    if (turn % 2 == 1)
        std::cout << "Tape 2 is longer" << std::endl;
    else
        std::cout << "Tape 3 is longer" << std::endl;

    // and what if the input tape is sorted?
    if (tape2.GetNumberOfSeries() + tape3.GetNumberOfSeries() == 1)
        std::cout << "File already sorted" << std::endl;
    numberOfPhases = fibIndex - 2;
    if (currFib != tape2.GetNumberOfSeries() + tape3.GetNumberOfSeries())
        dummyCount = currFib - tape2.GetNumberOfSeries() - tape3.GetNumberOfSeries();

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