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

        for (int j = 0; j < len; j++)
        {
            r = rand();
            char c = r % 94 + 33;
            file << c;
        }
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

bool FileManager::ReadBlockFromFile(std::string filename, int blockNum, std::vector<std::string> &buffer)
{
    std::ifstream file;
    file.open(filename);
    std::string line; // to read not needed data
    int recordCount = 0;
    int maxRecords = BLOC_SIZE / RECORD_SIZE;
    int currBlockNum = 1;

    while (currBlockNum != blockNum && !file.eof())
    {
        for (int i = 0; i < maxRecords; i++)
            getline(file, line);
        currBlockNum++;
        if (file.eof())
        {
            // std::cout << "Block: " << blockNum << " out of range" << std::endl;
            file.close();
            return false;
        }
    }

    // Remove not needed data
    ClearBufferFromIndex(buffer, 0);

    while (recordCount < maxRecords && !file.eof())
    {
        file >> buffer[recordCount];
        recordCount++;
        // std::cout << "Record: " << recordCount<< " " << buffer[recordCount - 1] << std::endl;
    }
    if (file.eof() && recordCount < maxRecords && buffer[recordCount] != "") // clear unused cells
        ClearBufferFromIndex(buffer, recordCount);

    file.close();
    return true;
}
void FileManager::WriteBlockToFile(std::string filename, std::vector<std::string> &buffer)
{
    std::ofstream file;
    file.open(filename, std::ios_base::app);

    for (size_t i = 0; i < buffer.size(); i++)
        if (buffer[i] != EMPTY_RECORD)
            file << buffer[i] << std::endl;

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
