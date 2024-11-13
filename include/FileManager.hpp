#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <vector>

// standard page size
#define BLOC_SIZE 4096 // 136 records, 16 bytes empty
#define RECORD_SIZE 30


// by default use this
#define TAPE1 "./tapes/tape1.txt"
#define TAPE2 "./tapes/tape2.txt"
#define TAPE3 "./tapes/tape3.txt"

// it is smaller than any other string since it only contains \0
#define EMPTY_RECORD ""

class FileManager
{

public:
    static FileManager &GetInstance();
    void GenerateFile(int lineCount, std::string filename);
    void GenerateEmptyFile(std::string filename);
    void DisplayEntireFile(std::string filename);
    bool ReadBlockFromFile(std::string filename, int blockNum, std::vector<std::string> &buffer);
    void WriteBlockToFile(std::string filename, std::vector<std::string> &buffer);
    void ClearFile(std::string filename);

    inline size_t GetReads() { return reads; }
    inline size_t GetWrites() { return writes; }

private:
    void ClearBufferFromIndex(std::vector<std::string> &buffer, size_t index);

private:
    FileManager();
    ~FileManager();

private:
    size_t reads = 0;
    size_t writes = 0;
};

#endif
