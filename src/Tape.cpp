#include "Tape.hpp"

Tape::Tape() { vectorOfRecords = std::vector<std::string>(BLOC_SIZE / RECORD_SIZE); }

Tape::~Tape() { vectorOfRecords.clear(); }

std::string Tape::GetNextRecord()
{
    if (index < BLOC_SIZE / RECORD_SIZE)
    {
        if (index > 0)
            prevRecord = vectorOfRecords[index - 1];
        index++;
        if (vectorOfRecords[index - 1] != EMPTY_RECORD)
            return vectorOfRecords[index - 1];
        else
        {
            hasEnded = true;
            return EMPTY_RECORD;
        }
    }
    if (index == BLOC_SIZE / RECORD_SIZE && vectorOfRecords[index - 1] != EMPTY_RECORD)
    {
        prevRecord = vectorOfRecords[index - 1];
        index = 0;
        blockNum++;
        FileManager::GetInstance().ReadBlockFromFile(filename, blockNum, vectorOfRecords);
        if (vectorOfRecords[index] != EMPTY_RECORD)
        {
            index++; // increment index since we return the first element of new block
            return vectorOfRecords[index - 1];
        }
    }
    if (vectorOfRecords[index] == EMPTY_RECORD)
        hasEnded = true;

    return EMPTY_RECORD;
}

std::string Tape::GetRecordAhead()
{
    // since we first increment the index while getting next record
    // we pick vectorOfRecords[index]
    if (index < BLOC_SIZE / RECORD_SIZE)
        return vectorOfRecords[index];
    else
    {
        prevRecord = vectorOfRecords[index - 1];
        index = 0;
        blockNum++;
        FileManager::GetInstance().ReadBlockFromFile(filename, blockNum, vectorOfRecords);
        if (vectorOfRecords[index] != EMPTY_RECORD)
            return vectorOfRecords[index];
    }
    return EMPTY_RECORD;
}

void Tape::SetNextRecord(std::string newRecord)
{
    if (newRecord == EMPTY_RECORD)
        return;
    if (index < BLOC_SIZE / RECORD_SIZE)
    {
        vectorOfRecords[index] = newRecord;
        index++;
        return;
    }
    if (index == BLOC_SIZE / RECORD_SIZE)
    {
        FileManager::GetInstance().WriteBlockToFile(filename, vectorOfRecords);
        index = 0;
        vectorOfRecords[index] = newRecord;
        index++;
        return;
    }
}

void Tape::SetNextRecordAndSortSerie(std::string newRecord, bool start)
{
    // simple insert if new serie begins
    if (start)
        SetNextRecord(newRecord);
    else
    {
        if (newRecord == EMPTY_RECORD)
            return;
        if (index < BLOC_SIZE / RECORD_SIZE)
        {
            size_t currPos = 0;
            if (index != 0)
                currPos = index - 1;
            if (newRecord < vectorOfRecords[currPos])
            {
                while (newRecord < vectorOfRecords[currPos])
                {
                    // move the record up
                    vectorOfRecords[currPos + 1] = vectorOfRecords[currPos];
                    vectorOfRecords[currPos] = newRecord;
                    if (currPos == 0)
                        break;
                    currPos--;
                }
                // vectorOfRecords[currPos + 1] = newRecord;
                index++;
                return;
            }
            else // in order so use the regular insert
                SetNextRecord(newRecord);
            return;
        }
        if (index == BLOC_SIZE / RECORD_SIZE)
        {
            size_t currPos = index - 1;
            if (newRecord < vectorOfRecords[currPos])
            {
                std::string recordToMove = vectorOfRecords[currPos];
                while (newRecord < vectorOfRecords[currPos] && recordToMove >= vectorOfRecords[currPos])
                {
                    if (currPos < BLOC_SIZE / RECORD_SIZE - 1)
                    {
                        vectorOfRecords[currPos + 1] = vectorOfRecords[currPos];
                        vectorOfRecords[currPos] = newRecord;
                    }
                    else
                        vectorOfRecords[currPos] = newRecord;
                    if (currPos == 0)
                        break;
                    currPos--;
                }
                FileManager::GetInstance().WriteBlockToFile(filename, vectorOfRecords);
                index = 0;
                vectorOfRecords[index] = recordToMove;
                index++;
                return;
            }
            else // in order so use the regular insert
                SetNextRecord(newRecord);
            return;
        }
    }
}

std::string Tape::GetSerieEnd()
{
    seriesEndIndex++;
    if (seriesEndIndex <= seriesEnd.size())
        return seriesEnd[seriesEndIndex - 1];
    else
        return EMPTY_RECORD;
}

std::string Tape::GetSerieNextEnd()
{
    if (seriesEndIndex < seriesEnd.size())
        return seriesEnd[seriesEndIndex];
    else
        return EMPTY_RECORD;
}

void Tape::SetNextSerieEnd(std::string newSerieEnd)
{
    seriesEnd.push_back(newSerieEnd);
}

void Tape::ResetIndexAndBuffer()
{
    ClearBuffer();
    index = 0;
    seriesEndIndex = 0;
    blockNum = 1;
    prevRecord = EMPTY_RECORD;
    hasEnded = false;
}

void Tape::Save() { FileManager::GetInstance().WriteBlockToFile(filename, vectorOfRecords); }
void Tape::SetFileAndFillBuffer(std::string filename)
{
    this->filename = filename;
    FileManager::GetInstance().ReadBlockFromFile(filename, blockNum, vectorOfRecords);
    index = 0;
    seriesEndIndex = 0;
    blockNum = 1;
    prevRecord = EMPTY_RECORD;
    hasEnded = false;
}