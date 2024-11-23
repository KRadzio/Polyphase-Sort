#include <iostream>

#include "FileManager.hpp"
#include "Sorter.hpp"
#include "App.hpp"

int main()
{
    //FileManager::GetInstance().GenerateFile(100000, "./tapes/test5.txt");
    //Sorter::GetInstance().Sort("./tapes/test5.txt");
    //std::cout << "Reads: " << FileManager::GetInstance().GetReads() << " Writes: " << FileManager::GetInstance().GetWrites() << " Files accessed " << FileManager::GetInstance().GetFileAccesses() << " times" << std::endl;
    App::GetInstance().MainLoop();
    return 0;
}