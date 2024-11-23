#include "App.hpp"

App::App()
{
    initscr();
    start_color();
}
App::~App() { endwin(); }

App &App::GetInstance()
{
    static App *instance = new App;
    return *instance;
}

void App::Experiment()
{
    Sorter::GetInstance().Sort("./tapes/test.txt", true);
    Sorter::GetInstance().Sort("./tapes/test2.txt", true);
    Sorter::GetInstance().Sort("./tapes/test3.txt", true);
    Sorter::GetInstance().Sort("./tapes/test4.txt", true);
    Sorter::GetInstance().Sort("./tapes/test5.txt", true);
}

void App::MainLoop()
{

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
            GenerateRecords();
            clear();
            break;
        case 0x32:
            LoadFile();
            clear();
            break;
        case 0x1B:
            run = false;
            break;

        default:
            clear();
            break;
        }
    }
}

void App::GenerateRecords()
{
    clear();
    char c = 0x00;
    std::string filepathRandom = "./tapes/random.txt";
    std::string filepathManual = "./tapes/manual.txt";
    std::vector<std::string> records;
    char buff[100];
    int i = 0;
    int num;
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
            printw("Insert number of records\n");
            refresh();
            curs_set(1);
            echo();
            getstr(buff);
            num = atoi(buff);
            while (i < num)
            {
                getstr(buff);
                records.push_back(buff);
                i++;
            }
            FileManager::GetInstance().WriteVectorToFile(filepathManual, records);
            curs_set(0);
            noecho();
            break;
        case 0x32:
            printw("Insert number of records\n");
            refresh();
            curs_set(1);
            echo();
            getstr(buff);
            FileManager::GetInstance().GenerateFile(atoi(buff), filepathRandom);
            curs_set(0);
            noecho();
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
            noecho();
            Sorter::GetInstance().Sort(path);
            printw("Press any key to continue\n");
            refresh();
            getchar();
            c = 0x1B;
            break;
        default:
            clear();
            break;
        }
    }
}

void App::GenerateChart() {}
