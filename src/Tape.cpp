#include "Tape.hpp"

Tape::Tape() { vectorOfRecords = std::vector<std::string>(BLOC_SIZE / RECORD_SIZE); }

Tape::~Tape() { vectorOfRecords.clear(); }

std::string Tape::GetNextRecord()
{
    if (index < BLOC_SIZE / RECORD_SIZE && vectorOfRecords[index] != EMPTY_RECORD)
    {
        if (index > 0)
            prevRecord = vectorOfRecords[index - 1];
        index++;
        if (vectorOfRecords[index - 1] != EMPTY_RECORD)
            return vectorOfRecords[index - 1];
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
            return vectorOfRecords[index-1];
        }   
    }
    if (vectorOfRecords[index] == EMPTY_RECORD)
        hasEnded = true;

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

std::string Tape::GetSerieNextEnd()
{
    seriesEndIndex++;
    return seriesEnd[seriesEndIndex - 1];
}

void Tape::SetNextSerieEnd(std::string newSerieEnd)
{
    seriesEnd.push_back(newSerieEnd);
}

void Tape::Save() { FileManager::GetInstance().WriteBlockToFile(filename, vectorOfRecords); }
void Tape::SetFileAndFillBuffer(std::string filename)
{
    this->filename = filename;
    FileManager::GetInstance().ReadBlockFromFile(filename, blockNum, vectorOfRecords);
}