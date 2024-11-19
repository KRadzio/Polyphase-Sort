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
        prevRecord = vectorOfRecords[BLOC_SIZE / RECORD_SIZE - 1];
        FileManager::GetInstance().WriteBlockToFile(filename, vectorOfRecords);
        index = 0;
        blockNum++;
    }
    vectorOfRecords[index] = newRecord;
    index++;
}

void Tape::SetNextRecordAndSortSerie(std::string newRecord)
{
    // do not insert a empty record
    if (newRecord == EMPTY_RECORD)
        return;
    if (index != 0)
    {
        if (index == BLOC_SIZE / RECORD_SIZE)
        {
            std::string tmp = vectorOfRecords[index - 1];
            if (newRecord < tmp)
            {
                vectorOfRecords[index - 1] = newRecord;
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
                if (currPos == 0) // check previous blocks
                    checkPrevBlocks(newRecord, currPos);

                prevRecord = vectorOfRecords[BLOC_SIZE / RECORD_SIZE - 1];
                FileManager::GetInstance().WriteBlockToFile(filename, vectorOfRecords);
                index = 0;
                blockNum++;
                vectorOfRecords[index] = tmp;
                index++;

                return;
            }
            else
            {
                prevRecord = vectorOfRecords[BLOC_SIZE / RECORD_SIZE - 1];
                FileManager::GetInstance().WriteBlockToFile(filename, vectorOfRecords);
                index = 0;
                blockNum++;
                vectorOfRecords[index] = newRecord;
                index++;
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
            if (currPos == 0) // check previous blocks
                checkPrevBlocks(newRecord, currPos);
            index++;
            return;
        }
        else // in order
            SetNextRecord(newRecord);
    }
    else if (index == 0 && blockNum == 1) // standard insert
        SetNextRecord(newRecord);
    else
    {
        if (newRecord < prevRecord) // we save the prev record from last block
            checkPrevBlocks(newRecord, BLOC_SIZE / RECORD_SIZE - 1);
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

// this may need optimization
// sometimes it puts a record out of order at the top
void Tape::checkPrevBlocks(std::string newRecord, size_t currPos)
{
    size_t currBlockNum = blockNum;
    currBlockNum--;
    if (currBlockNum == 0) // no need to check further
        return;
    if (currBlockNum > 0)
        FileManager::GetInstance().ReadBlockFromFile(filename, currBlockNum, swapBuffer);
    if (newRecord < swapBuffer[currPos])
    {
        vectorOfRecords[0] = swapBuffer[currPos];
        swapBuffer[currPos] = newRecord;
        currPos--;
    }
    else // we found the place
        return;

    while (newRecord < swapBuffer[currPos])
    {
        swapBuffer[currPos + 1] = swapBuffer[currPos];
        swapBuffer[currPos] = newRecord;
        currPos--;
        if (currPos == 0)
        {
            // at 0
            swapBuffer[currPos + 1] = swapBuffer[currPos];
            swapBuffer[currPos] = newRecord;
            for (size_t i = 0; i < swapBuffer.size(); i++)
                tmpV[i] = swapBuffer[i];
            currPos = BLOC_SIZE / RECORD_SIZE - 1;
            currBlockNum--;
            if (currBlockNum == 0)
                break;
            if (currBlockNum > 0)
                FileManager::GetInstance().ReadBlockFromFile(filename, currBlockNum, swapBuffer);
            if (newRecord < swapBuffer[currPos])
            {
                tmpV[0] = swapBuffer[currPos];
                FileManager::GetInstance().ReplaceBlockInFile(filename, currBlockNum + 1, tmpV);
                swapBuffer[currPos] = newRecord;
                currPos--;
            }
            else // we found the place
                break;
        }
    }
    FileManager::GetInstance().ReplaceBlockInFile(filename, currBlockNum, swapBuffer);
}