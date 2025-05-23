#include "Tape.hpp"

Tape::Tape()
{
    vectorOfRecords = std::vector<std::string>(BLOC_SIZE / RECORD_SIZE);
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
    runsEndIndex++;
    if (runsEndIndex <= seriesEnd.size())
        return seriesEnd[runsEndIndex - 1];
    else
        return EMPTY_RECORD;
}

void Tape::SetNextSerieEnd(std::string newSerieEnd)
{
    seriesEnd.push_back(newSerieEnd);
    runsEndIndex++;
}

void Tape::ResetIndex(bool save)
{
    if (blockNum > 1 && save)
    {
        FileManager::GetInstance().WriteBlockToFile(filename, vectorOfRecords);
        FileManager::GetInstance().ReadBlockFromFile(filename, 1, vectorOfRecords);
    }
    index = 0;
    runsEndIndex = 0;
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
    runsEndIndex = 0;
    blockNum = 1;
    prevRecord = EMPTY_RECORD;
    hasEnded = false;
}

void Tape::ClearBuffer()
{
    size_t i = 0;
    while (i != vectorOfRecords.size())
    {
        vectorOfRecords[i] = EMPTY_RECORD;
        i++;
    }
}

void Tape::Diplay()
{
    std::ifstream file;
    std::string line;
    char buffer[31] = {'\0'};
    file.open(filename);
    for (size_t i = 1; i < blockNum; i++)
    {
        for (int j = 0; j < BLOC_SIZE / RECORD_SIZE; j++)
        {
            for (size_t k = 0; k < 31; k++)
                buffer[k] = UNUSED_BYTE;
            getline(file, line);
            for (size_t k = 0; k < line.size(); k++)
            {
                if (line[k] == UNUSED_BYTE)
                    buffer[k] = UNUSED_BYTE;
                else
                    buffer[k] = line[k];
            }

            wprintw(window, buffer);
            wprintw(window, "\n");
            wrefresh(window);
        }
    }
    file.close();

    for (size_t i = 0; i < vectorOfRecords.size(); i++)
    {
        if (vectorOfRecords[i] != EMPTY_RECORD)
        {
            for (size_t k = 0; k < 31; k++)
                buffer[k] = UNUSED_BYTE;
            for (size_t k = 0; k < vectorOfRecords[i].size(); k++)
            {
                if (vectorOfRecords[i][k] == UNUSED_BYTE)
                    buffer[k] = UNUSED_BYTE;
                else
                    buffer[k] = vectorOfRecords[i][k];
            }
            wprintw(window, buffer);
            wprintw(window, "\n");
            wrefresh(window);
        }
    }
}