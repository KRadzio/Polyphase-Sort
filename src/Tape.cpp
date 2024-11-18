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
        blockNum++;
        vectorOfRecords[index] = newRecord;
        index++;
        return;
    }
}

void Tape::SetNextRecordAndSortSerie(std::string newRecord, bool startS, bool startL)
{
    // simple insert if new serie begins
    if (startS)
        SetNextRecord(newRecord);
    else if (startL) // default insert
    {
        size_t currPos = 0;
        if (index != 0)
            currPos = index - 1;
        if (newRecord < vectorOfRecords[currPos])
        {
            if (currPos == BLOC_SIZE / RECORD_SIZE - 1)
            {
                std::string tmp = vectorOfRecords[currPos];
                vectorOfRecords[currPos] = newRecord;
                index = 0;
                FileManager::GetInstance().WriteBlockToFile(filename, vectorOfRecords);
                vectorOfRecords[index] = tmp;
                blockNum++;
                index++;
                return;
            }
            else
            {
                vectorOfRecords[currPos + 1] = vectorOfRecords[currPos];
                vectorOfRecords[currPos] = newRecord;
                index++;
                return;
            }
        }
        else // in order so use the regular insert
            SetNextRecord(newRecord);
    }
    else
    {
        // also check if it is in previous blocks
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
                if (currPos == 0) // check previous blocks
                    checkPrevBlocks(newRecord, currPos);
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
                if (currPos == 0) // check prev blocks
                    checkPrevBlocks(newRecord, currPos);

                FileManager::GetInstance().WriteBlockToFile(filename, vectorOfRecords);
                index = 0;
                vectorOfRecords[index] = recordToMove;
                blockNum++;
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
    seriesEndIndex++;
}

void Tape::ResetIndex(bool save )
{
    if(blockNum > 1 && save)
    {
        FileManager::GetInstance().WriteBlockToFile(filename, vectorOfRecords);
        FileManager::GetInstance().ReadBlockFromFile(filename,1, vectorOfRecords);
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
    currPos = BLOC_SIZE / RECORD_SIZE - 1;
    currBlockNum--;
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
    if(currBlockNum == 0) // this may happen
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
            if (currBlockNum > 0)
                FileManager::GetInstance().ReadBlockFromFile(filename, currBlockNum, swapBuffer);
            if (newRecord < swapBuffer[currPos])
            {
                tmpV[0] = swapBuffer[currPos];
                FileManager::GetInstance().ReplaceBlockInFile(filename, currBlockNum + 1, tmpV);
                swapBuffer[currPos] = newRecord;
                currPos--;
            }
        }
    }
    FileManager::GetInstance().ReplaceBlockInFile(filename, currBlockNum, swapBuffer);
}