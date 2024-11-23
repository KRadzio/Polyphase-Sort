#include "FileManager.hpp"

FileManager::FileManager() {}
FileManager::~FileManager() {}

FileManager &FileManager::GetInstance()
{
    static FileManager *instance = new FileManager;
    return *instance;
}
void FileManager::GenerateFile(int lineCount, std::string filename)
{
    std::ofstream file;
    file.open(filename);
    srand(time(NULL));
    for (int i = 0; i < lineCount; i++)
    {
        int r = rand();
        int len = r % 30 + 1;
        int rest = RECORD_SIZE - len;

        for (int j = 0; j < len; j++)
        {
            r = rand();
            char c = r % 94 + 33;
            file << c;
        }
        for (int j = 0; j < rest; j++)
            file << UNUSED_BYTE;

        file << std::endl;
    }
    file.close();
}

void FileManager::WriteVectorToFile(std::string filename, std::vector<std::string> &vecotr)
{
    std::ofstream file;
    file.open(filename);
    for (size_t i = 0; i < vecotr.size(); i++)
    {
        size_t rest = RECORD_SIZE - vecotr[i].size();
        file << vecotr[i];
        for (size_t j = 0; j < rest; j++)
            file << UNUSED_BYTE;
        file << std::endl;
    }
    file.close();
}

void FileManager::GenerateEmptyFile(std::string filename)
{
    std::ofstream file;
    file.open(filename);
    file.close();
}

void FileManager::DisplayEntireFile(std::string filename)
{
    std::ifstream file;
    file.open(filename);
    std::string line;

    int blockNum = 0;
    int currBytesInBlock = 0;
    while (getline(file, line))
    {
        currBytesInBlock += 30;
        if (currBytesInBlock > BLOC_SIZE)
        {
            currBytesInBlock = 0;
            blockNum++;
        }
        std::cout << "Block: " << blockNum << " Offset: " << currBytesInBlock << " Line: " << line << std::endl;
    }
    file.close();
}

void FileManager::ReadBlockFromFile(std::string filename, int blockNum, std::vector<std::string> &buffer)
{
    std::ifstream file;
    file.open(filename);
    std::string line; // to read not needed data
    int recordCount = 0;
    int maxRecords = BLOC_SIZE / RECORD_SIZE;
    int currBlockNum = 1;

    // get to the block that we want to read
    while (currBlockNum < blockNum && !file.eof())
    {
        for (int i = 0; i < maxRecords; i++)
            getline(file, line);
        currBlockNum++;
        if (file.eof()) // out of bounds
        {
            file.close();
            ClearBufferFromIndex(buffer, 0);
            return;
        }
    }

    reads += BLOC_SIZE / RECORD_SIZE;
    fileAcceses++;
    // Remove not needed data
    ClearBufferFromIndex(buffer, 0);

    while (recordCount < maxRecords && !file.eof())
    {
        std::string line;
        getline(file, line);
        std::stringstream ss(line);
        // read as records with diffrent lengths, but we save with a fixed length
        getline(ss, line, UNUSED_BYTE);
        buffer[recordCount] = line;
        recordCount++;
    }
    if (file.eof() && recordCount < maxRecords && buffer[recordCount] != "") // clear unused cells
        ClearBufferFromIndex(buffer, recordCount);

    file.close();
    return;
}

void FileManager::WriteBlockToFile(std::string filename, std::vector<std::string> &buffer)
{
    std::ofstream file;
    file.open(filename, std::ios_base::app);

    // every record needs to have a fixed number of characters
    for (size_t i = 0; i < buffer.size(); i++)
    {
        if (buffer[i] != EMPTY_RECORD)
        {
            file << buffer[i];
            for (size_t j = buffer[i].size(); j < RECORD_SIZE; j++)
                file << UNUSED_BYTE;
            file << std::endl;
        }
    }

    writes += BLOC_SIZE / RECORD_SIZE;
    fileAcceses++;

    file.close();
    ClearBufferFromIndex(buffer, 0);
}

void FileManager::ReplaceBlockInFile(std::string filename, int blockNum, std::vector<std::string> &buffer)
{
    std::fstream file;
    file.open(filename);

    int currBlockNum = 1;
    int maxRecords = BLOC_SIZE / RECORD_SIZE;
    std::string line;

    // get to the block that we want to replace
    while (currBlockNum != blockNum)
    {
        for (int i = 0; i < maxRecords; i++)
            getline(file, line);
        currBlockNum++;
        if (file.eof()) // out of bounds
        {
            file.close();
            return;
        }
    }

    writes += BLOC_SIZE / RECORD_SIZE;
    fileAcceses++;

    // every record needs to have a fixed number of characters
    for (size_t i = 0; i < buffer.size(); i++)
    {
        if (buffer[i] != EMPTY_RECORD)
        {
            file << buffer[i];
            for (size_t j = buffer[i].size(); j < RECORD_SIZE; j++)
                file << UNUSED_BYTE;
            file << std::endl;
        }
    }

    file.close();
    ClearBufferFromIndex(buffer, 0);
}

void FileManager::ClearFile(std::string filename)
{
    std::ofstream file;
    file.open(filename, std::ios_base::trunc);
    file.close();
}

void FileManager::ClearBufferFromIndex(std::vector<std::string> &buffer, size_t index)
{
    while (index != buffer.size())
    {
        buffer[index] = "";
        index++;
    }
}
