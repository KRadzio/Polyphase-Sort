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
        return vectorOfRecords[index - 1];
    }
    if (index == BLOC_SIZE / RECORD_SIZE)
    {
        prevRecord = vectorOfRecords[index - 1];
        index = 0;
        blockNum++;
        FileManager::GetInstance().ReadBlockFromFile(filename, blockNum, vectorOfRecords);
        return vectorOfRecords[index];
    }
    if (vectorOfRecords[index] == EMPTY_RECORD)
        hasEnded = true;
    // the record is empty
    return EMPTY_RECORD;
}

void Tape::SetNextRecord(std::string newRecord)
{
    if (index < BLOC_SIZE / RECORD_SIZE - 1)
    {
        vectorOfRecords[index] = newRecord;
        index++;
    }
    if (index == BLOC_SIZE / RECORD_SIZE - 1)
    {
        vectorOfRecords[index] = newRecord;
        FileManager::GetInstance().WriteBlockToFile(filename, vectorOfRecords);
        index = 0;
    }
    if (newRecord == EMPTY_RECORD) // the input has ended so we save what we have in buffer to file
        FileManager::GetInstance().WriteBlockToFile(filename, vectorOfRecords);
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