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

void Sorter::Sort(std::string inputTapeName, bool skip)
{
    ResetTapes(); // so that we can sort onece again

    SplitToTapes(inputTapeName, skip); // dystribution

    // sorting

    SetUpTapesBeforeSorting();

    size_t currPhase = 0;
    std::string recordS = EMPTY_RECORD; // if we reached the end of shorter tape this will be reseted to EMPTY_RECORD
    std::string recordL = EMPTY_RECORD;
    char c = 0x00;

    while (currPhase < numberOfPhases)
    {
        size_t runsCount = 0;

        while (runsCount < Fib(numberOfPhases - currPhase))
        {
            // we first remove the dummy runs
            if (dummyCount > 0)
            {
                dummyCount--;
                FillWithShorterTapeRuns(recordS);
            }
            else
                MergeTwoRuns(recordS, recordL);
            runsCount++;
        }
        currPhase++;
        if (currPhase < numberOfPhases)
        {
            SwapAndClearTapes();
            recordS = recordL;
            recordL = EMPTY_RECORD;
        }
        if (!skip)
        {
            wprintw(window, "Phase %ld ended\n", currPhase);
            wprintw(window, "p) Print tapes\n");
            wrefresh(window);
            c = wgetch(window);
            switch (c)
            {
            case 0x70:
                longerTape->Diplay();
                break;
            default:
                break;
            }
        }
    }
    if (!skip)
    {
        wprintw(window, "After sorting:\n");
        wrefresh(window);

        wprintw(window, "p) Print tapes\n");
        wrefresh(window);
        c = wgetch(window);
        switch (c)
        {
        case 0x70:
            emptyTape->Diplay();
            break;
        default:
            break;
        }
    }
    // save since it may not have been saved
    emptyTape->Save();

    reads = FileManager::GetInstance().GetReads();
    writes = FileManager::GetInstance().GetWrites();
    fileAccesses = FileManager::GetInstance().GetFileAccesses();
    fileAccesesT = 2 * recordsCount * (1.04 * log2(tape2.GetNumberOfRuns() + tape3.GetNumberOfRuns()) + 1) / (BLOC_SIZE / RECORD_SIZE);
    phasesT = 1.45 * log2(tape2.GetNumberOfRuns() + tape3.GetNumberOfRuns());
    wprintw(window, "Theoretical number of file accesses %f\n", fileAccesesT);
    wprintw(window, "Theoretical number of phases: %f\n", phasesT);
    wprintw(window, "Reads: %ld Writes: %ld\n", reads, writes);
    wprintw(window, "File accesed: %ld times\n", fileAccesses);
    wprintw(window, "Number of phases: %ld\n", numberOfPhases);
    wrefresh(window);
}

void Sorter::SetWindow(_win_st *window)
{
    this->window = window;
    tape1.SetWindow(window);
    tape2.SetWindow(window);
    tape3.SetWindow(window);
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

void Sorter::SplitToTapes(std::string inputTapeName, bool skip)
{
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
    if (currFib != tape2.GetNumberOfRuns() + tape3.GetNumberOfRuns())
        dummyCount = currFib - tape2.GetNumberOfRuns() - tape3.GetNumberOfRuns();

    initialRunsCount = tape2.GetNumberOfRuns() + tape3.GetNumberOfRuns();

    if (!skip)
    {
        wprintw(window, "Before sorting:\n");
        wrefresh(window);

        char c;
        wprintw(window, "p) Print tapes\n");
        wrefresh(window);
        c = wgetch(window);
        switch (c)
        {
        case 0x70:
            tape1.Diplay();
            break;
        default:
            break;
        }
    }
}

void Sorter::FillTapeUpToCurrGoal(Tape &currTape, Tape &otherTape)
{
    while (currTape.GetNumberOfRuns() < currFib)
    {
        std::string record = tape1.GetNextRecord();
        std::string prevRecord = tape1.GetPrevRecord();
        if (record == EMPTY_RECORD)
        {
            currTape.SetNextSerieEnd(prevRecord);
            currTape.IncrementNumberOfRuns();
            break;
        }
        if (prevRecord > record)
        {
            currTape.SetNextSerieEnd(prevRecord);
            currTape.IncrementNumberOfRuns();
            // we will write the record to the other tape if we reached the current goal
            if (currTape.GetNumberOfRuns() == currFib)
                otherTape.SetNextRecord(record);
            else
                currTape.SetNextRecord(record);
        }
        else
            currTape.SetNextRecord(record);
        recordsCount++;
    }
}

void Sorter::SetUpTapesBeforeSorting()
{
    // clear tape one since it will be empty
    tape1.SetFile(TAPE1);
    tape1.ResetIndex(false);
    tape1.ClearBuffer();
    tape1.Clear();
    emptyTape = &tape1;

    // pick the shorter one
    if (tape2.GetNumberOfRuns() == Fib(fibIndex - 2))
    {
        shorterTape = &tape2;
        longerTape = &tape3;
    }
    if (tape3.GetNumberOfRuns() == Fib(fibIndex - 2))
    {
        shorterTape = &tape3;
        longerTape = &tape2;
    }

    // reset index
    shorterTape->ResetIndex();
    longerTape->ResetIndex();
}

void Sorter::SwapAndClearTapes()
{
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

void Sorter::FillWithShorterTapeRuns(std::string &recordS)
{
    bool serieEndedS = false;
    std::string serieEndS = EMPTY_RECORD;
    serieEndS = shorterTape->GetSerieEnd();
    emptyTape->SetNextSerieEnd(serieEndS);

    // new run begin
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

void Sorter::MergeTwoRuns(std::string &recordS, std::string &recordL)
{
    bool serieEndedS = false;
    bool serieEndedL = false;
    std::string serieEndS = shorterTape->GetSerieEnd();
    std::string serieEndL = longerTape->GetSerieEnd();

    // set new run end
    if (serieEndS > serieEndL)
        emptyTape->SetNextSerieEnd(serieEndS);
    else
        emptyTape->SetNextSerieEnd(serieEndL);

    // new run begin
    if (recordS != EMPTY_RECORD && recordL != EMPTY_RECORD)
        InsertNextRecord(recordS, recordL);
    else if (recordS != EMPTY_RECORD) // after dummy runs
    {
        recordL = longerTape->GetNextRecord();
        InsertNextRecord(recordS, recordL);
    }
    else if (recordL != EMPTY_RECORD) // if shorter tape ended
    {
        recordS = shorterTape->GetNextRecord();
        InsertNextRecord(recordS, recordL);
    }
    // merge two runs
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

void Sorter::ResetTapes()
{
    dummyCount = 0;
    fibIndex = 1;
    recordsCount = 0;
    FileManager::GetInstance().ResetCounters();

    tape1.Clear();
    tape1.ResetSeriesEnd();
    tape1.ClearBuffer();
    tape1.ResetIndex(false);
    tape1.ResetRunsCount();

    tape2.Clear();
    tape2.ResetSeriesEnd();
    tape2.ClearBuffer();
    tape2.ResetIndex(false);
    tape2.ResetRunsCount();

    tape3.Clear();
    tape3.ResetSeriesEnd();
    tape3.ClearBuffer();
    tape3.ResetIndex(false);
    tape3.ResetRunsCount();
}

int Sorter::FindClosestFibNumberIndex(size_t runsCount)
{
    int index = 1;
    for (int i = 0; i < 1000; i++) // find first fib number that is grater or equal to runsCount
    {
        if (Fib(i) >= runsCount)
        {
            index = i;
            break;
        }
    }
    return index;
}
