#ifndef APP_HPP
#define APP_HPP

#include <iostream>

#include <ncurses.h>

#include "Sorter.hpp"

class App
{
public:
    static App& GetInstance();
    void MainLoop();
    void GenerateRecords();
    void LoadFile();

private:
    App();
    ~App();

private:
    bool run = true;
};

#endif
