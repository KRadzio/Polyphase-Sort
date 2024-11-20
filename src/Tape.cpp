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