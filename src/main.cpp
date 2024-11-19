#include <iostream>

#include "FileManager.hpp"
#include "Sorter.hpp"

int main()
{
    // FileManager::GetInstance().GenerateFile(1000, "./tapes/test7.txt");
    Sorter::GetInstance().Sort("./tapes/test4.txt");
    // 10 times more file acceses
    std::cout << "Reads: " << FileManager::GetInstance().GetReads() << " Writes: " << FileManager::GetInstance().GetWrites() << " Files accessed " << FileManager::GetInstance().GetFileAccesses() << " times" << std::endl;
    return 0;
}