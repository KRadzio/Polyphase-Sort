#ifndef APP_HPP
#define APP_HPP

#include <iostream>

#include <ncurses.h>
#include <plplot/plplot.h>

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
    void GenerateChart();

private:
    App();
    ~App();

private:
    bool run = true;
};

#endif
