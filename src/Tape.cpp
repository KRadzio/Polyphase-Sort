#include "Tape.hpp"

Tape::Tape() { vectorOfRecords = std::vector<std::string>(BLOC_SIZE / RECORD_SIZE); }
Tape::Tape(std::string filename)
{
    vectorOfRecords = std::vector<std::string>(BLOC_SIZE / RECORD_SIZE);
    this->fileName = fileName;
}
Tape::~Tape() { vectorOfRecords.clear(); }

std::string Tape::GetNextRecord()
{
    if (index < BLOC_SIZE / RECORD_SIZE - 1 && vectorOfRecords[index] != EMPTY_RECORD)
    {
        index++;
        return vectorOfRecords[index - 1];
    }
    if (index == BLOC_SIZE / RECORD_SIZE - 1 && vectorOfRecords[index] != EMPTY_RECORD)
    {
        lastRecordFromPreviousBlock = vectorOfRecords[index];
        std::string record = vectorOfRecords[index];
        index = 0;
        blockNum++;
        FileManager::GetInstance().ReadBlockFromFile(fileName, blockNum, vectorOfRecords);
        return record;
    }
    // the record is empty
    return EMPTY_RECORD;
}

std::string Tape::GetSerieNextEnd()
{
    seriesEndIndex++;
    return seriesEnd[seriesEndIndex - 1];
}