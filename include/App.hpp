#ifndef APP_HPP
#define APP_HPP

#include <iostream>

#include <ncurses.h>

#include "Sorter.hpp"

class App
{
public:
    static App &GetInstance();
    void Experiment();
    void MainLoop();

private:
    void GenerateRecords();
    void LoadFile();
    void RunTestOnTape(std::string filename, size_t N);

private:
    App();
    ~App();

private:
    bool run = true;
    WINDOW *window;
    int height;
    int width;
    std::ofstream outputFile;
};

#endif
