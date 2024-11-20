#include "Tape.hpp"

Tape::Tape()
{
    vectorOfRecords = std::vector<std::string>(BLOC_SIZE / RECORD_SIZE);
    swapBuffer = std::vector<std::string>(BLOC_SIZE / RECORD_SIZE);
    tmpV = std::vector<std::string>(BLOC_SIZE / RECORD_SIZE);
}

Tape::~Tape() { vectorOfRecords.clear(); }

std::string Tape::GetNextRecord()
{
    if (index > 0)
        prevRecord = vectorOfRecords[index - 1];
    if (index == BLOC_SIZE / RECORD_SIZE)
    {
        prevRecord = vectorOfRecords[index - 1];
        index = 0;
        blockNum++;
        FileManager::GetInstance().ReadBlockFromFile(filename, blockNum, vectorOfRecords);
    }

    std::string tmp = vectorOfRecords[index];
    index++;

    if (tmp == EMPTY_RECORD)
        hasEnded = true;

    return tmp;
}

void Tape::SetNextRecord(std::string newRecord)
{
    if (index == BLOC_SIZE / RECORD_SIZE)
    {
        FileManager::GetInstance().WriteBlockToFile(filename, vectorOfRecords);
        index = 0;
        blockNum++;
    }
    vectorOfRecords[index] = newRecord;
    index++;
}

void Tape::SetNextRecordAndSortSerie(std::string newRecord)
{
    if (index == 0 && blockNum == 1) // standard insert
    {
        SetNextRecord(newRecord);
        return;
    }

    if (index == BLOC_SIZE / RECORD_SIZE)
    {
        std::string tmp = vectorOfRecords[BLOC_SIZE / RECORD_SIZE - 1];
        if (newRecord < tmp)
        {
            size_t currPos = index - 2;
            while (newRecord < vectorOfRecords[currPos])
            {
                // move the record up
                vectorOfRecords[currPos + 1] = vectorOfRecords[currPos];
                vectorOfRecords[currPos] = newRecord;
                if (currPos == 0)
                    break;
                currPos--;
            }
            FileManager::GetInstance().WriteBlockToFile(filename, vectorOfRecords);
            index = 1;
            blockNum++;
            vectorOfRecords[0] = tmp;
            return;
        }
        else
        {
            FileManager::GetInstance().WriteBlockToFile(filename, vectorOfRecords);
            index = 1;
            blockNum++;
            vectorOfRecords[0] = newRecord;
            return;
        }
    }

    size_t currPos = index - 1;
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
        index++;
        return;
    }
    else // in order
        SetNextRecord(newRecord);
}

std::string Tape::GetSerieEnd()
{
    seriesEndIndex++;
    if (seriesEndIndex <= seriesEnd.size())
        return seriesEnd[seriesEndIndex - 1];
    else
        return EMPTY_RECORD;
}

void Tape::SetNextSerieEnd(std::string newSerieEnd)
{
    seriesEnd.push_back(newSerieEnd);
    seriesEndIndex++;
}

void Tape::ResetIndex(bool save)
{
    if (blockNum > 1 && save)
    {
        FileManager::GetInstance().WriteBlockToFile(filename, vectorOfRecords);
        FileManager::GetInstance().ReadBlockFromFile(filename, 1, vectorOfRecords);
    }
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