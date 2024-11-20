#include <iostream>

#include "FileManager.hpp"
#include "Sorter.hpp"

int main()
{
    FileManager::GetInstance().GenerateFile(10000, "./tapes/test5.txt");
    Sorter::GetInstance().Sort("./tapes/test5.txt");
    // 10 times more file acceses
    std::cout << "Reads: " << FileManager::GetInstance().GetReads() << " Writes: " << FileManager::GetInstance().GetWrites() << " Files accessed " << FileManager::GetInstance().GetFileAccesses() << " times" << std::endl;
    return 0;
}