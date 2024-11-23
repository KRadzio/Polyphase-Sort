#include "App.hpp"

App::App() {}
App::~App() {}

App &App::GetInstance()
{
    static App *instance = new App;
    return *instance;
}

void App::MainLoop()
{
    initscr();
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    raw();
    noecho();
    curs_set(0);
    char c;
    clear();
    while (run)
    {
        printw("1) Generate records\n");
        printw("2) Load records from file\n");
        printw("ESC) Quit\n");
        refresh();
        c = getch();

        switch (c)
        {
        case 0x31:
            // attroff(COLOR_PAIR(2));
            // attron(COLOR_PAIR(1));
            GenerateRecords();
            clear();
            break;
        case 0x32:
            LoadFile();
            clear();
            // attroff(COLOR_PAIR(1));
            // attron(COLOR_PAIR(2));
            break;
        case 0x1B:
            run = false;
            break;

        default:
            clear();
            break;
        }
    }
    endwin();
}

void App::GenerateRecords()
{
    clear();
    char c = 0x00;
    while (c != 0x1B)
    {
        printw("1) Manualy\n");
        printw("2) Randomly\n");
        printw("ESC) Return to main menu\n");
        refresh();
        c = getch();
        switch (c)
        {
        case 0x31:
            break;
        case 0x32:
            break;
        default:
            clear();
            break;
        }
    }
}
void App::LoadFile()
{
    clear();
    char c = 0x00;
    std::string path = "./tapes/";
    char buff[100];
    while (c != 0x1B)
    {
        printw("1) Insert file name\n");
        printw("ESC) Quit\n");
        refresh();
        c = getch();
        switch (c)
        {
        case 0x31:
            curs_set(1);
            echo();
            getstr(buff);
            curs_set(0);
            path += buff;
            Sorter::GetInstance().Sort(path);
            printw("Press any key to continue\n");
            refresh();
            getchar();
            noecho();
            c = 0x1B;
            break;
        default:
            clear();
            break;
        }
    }
}
