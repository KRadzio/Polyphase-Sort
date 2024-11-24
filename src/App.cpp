#include "App.hpp"

App::App()
{
    initscr();
    getmaxyx(stdscr, height, width);
    window = newwin(height - 2, width - 2, 1, 1);
    scrollok(window, TRUE);
    raw();
    noecho();
    curs_set(0);

    Sorter::GetInstance().SetWindow(window);
}
App::~App() {}

App &App::GetInstance()
{
    static App *instance = new App;
    return *instance;
}

void App::Experiment()
{
    wclear(window);
    wprintw(window, "N = 10\n");
    wrefresh(window);
    Sorter::GetInstance().Sort("./tapes/test.txt", true);
    wprintw(window, "%ld %ld\n", Sorter::GetInstance().GetInitialNumberOfSeries(), Sorter::GetInstance().GetNumberOfRecords());
    wrefresh(window);

    wprintw(window, "N = 100\n");
    wrefresh(window);
    Sorter::GetInstance().Sort("./tapes/test2.txt", true);
    wprintw(window, "%ld %ld\n", Sorter::GetInstance().GetInitialNumberOfSeries(), Sorter::GetInstance().GetNumberOfRecords());
    wrefresh(window);

    wprintw(window, "N = 500\n");
    wrefresh(window);
    Sorter::GetInstance().Sort("./tapes/test3.txt", true);
    wprintw(window, "%ld %ld\n", Sorter::GetInstance().GetInitialNumberOfSeries(), Sorter::GetInstance().GetNumberOfRecords());
    wrefresh(window);

    wprintw(window, "N = 1000\n");
    wrefresh(window);
    Sorter::GetInstance().Sort("./tapes/test4.txt", true);
    wprintw(window, "%ld %ld\n", Sorter::GetInstance().GetInitialNumberOfSeries(), Sorter::GetInstance().GetNumberOfRecords());
    wrefresh(window);

    wprintw(window, "N = 2000\n");
    wrefresh(window);
    Sorter::GetInstance().Sort("./tapes/test5.txt", true);
    wprintw(window, "%ld %ld\n", Sorter::GetInstance().GetInitialNumberOfSeries(), Sorter::GetInstance().GetNumberOfRecords());
    wrefresh(window);

    wprintw(window, "N = 5000\n");
    wrefresh(window);
    Sorter::GetInstance().Sort("./tapes/test6.txt", true);
    wprintw(window, "%ld %ld\n", Sorter::GetInstance().GetInitialNumberOfSeries(), Sorter::GetInstance().GetNumberOfRecords());
    wrefresh(window);

    wprintw(window, "N = 10000\n");
    wrefresh(window);
    Sorter::GetInstance().Sort("./tapes/test7.txt", true);
    wprintw(window, "%ld %ld\n", Sorter::GetInstance().GetInitialNumberOfSeries(), Sorter::GetInstance().GetNumberOfRecords());
    wrefresh(window);

    wprintw(window, "N = 20000\n");
    wrefresh(window);
    Sorter::GetInstance().Sort("./tapes/test8.txt", true);
    wprintw(window, "%ld %ld\n", Sorter::GetInstance().GetInitialNumberOfSeries(), Sorter::GetInstance().GetNumberOfRecords());
    wrefresh(window);

    wprintw(window, "N = 50000\n");
    wrefresh(window);
    Sorter::GetInstance().Sort("./tapes/test9.txt", true);
    wprintw(window, "%ld %ld\n", Sorter::GetInstance().GetInitialNumberOfSeries(), Sorter::GetInstance().GetNumberOfRecords());
    wrefresh(window);

    wprintw(window, "N = 100000\n");
    wrefresh(window);
    Sorter::GetInstance().Sort("./tapes/test10.txt", true);
    wprintw(window, "%ld %ld\n", Sorter::GetInstance().GetInitialNumberOfSeries(), Sorter::GetInstance().GetNumberOfRecords());
    wrefresh(window);

    wprintw(window, "Press any key to continue\n");
    wrefresh(window);
    getchar();

    echo();
    curs_set(1);
    endwin();
}

void App::MainLoop()
{
    char c;
    wclear(window);
    while (run)
    {
        wprintw(window, "1) Generate records\n");
        wprintw(window, "2) Load records from file\n");
        wprintw(window, "c) Clear screen\n");
        wprintw(window, "q) Quit\n");
        wrefresh(window);
        c = wgetch(window);
        switch (c)
        {
        case 0x31:
            GenerateRecords();
            wclear(window);
            break;
        case 0x32:
            LoadFile();
            break;
        case 0x71:
            run = false;
            break;
        case 0x63:
            wclear(window);
            break;
        default:
            break;
        }
    }
    echo();
    curs_set(1);
    endwin();
}

void App::GenerateRecords()
{
    wclear(window);
    char c = 0x00;
    std::string filepathRandom = "./tapes/random.txt";
    std::string filepathManual = "./tapes/manual.txt";
    std::vector<std::string> records;
    char buff[100];
    int i = 0;
    int num;
    while (c != 0x1B)
    {
        wprintw(window, "1) Manualy\n");
        wprintw(window, "2) Randomly\n");
        wprintw(window, "ESC) Return to main menu\n");
        wrefresh(window);
        c = wgetch(window);
        switch (c)
        {
        case 0x31:
            wprintw(window, "Insert number of records\n");
            wrefresh(window);
            curs_set(1);
            echo();
            wgetstr(window, buff);
            num = atoi(buff);
            while (i < num)
            {
                wgetstr(window, buff);
                records.push_back(buff);
                i++;
            }
            FileManager::GetInstance().WriteVectorToFile(filepathManual, records);
            curs_set(0);
            noecho();
            break;
        case 0x32:
            wprintw(window, "Insert number of records\n");
            wrefresh(window);
            curs_set(1);
            echo();
            wgetstr(window, buff);
            FileManager::GetInstance().GenerateFile(atoi(buff), filepathRandom);
            curs_set(0);
            noecho();
            break;
        default:
            wclear(window);
            break;
        }
    }
}

void App::LoadFile()
{
    wclear(window);
    char c = 0x00;
    std::string path = "./tapes/";
    char buff[100];
    while (c != 0x1B)
    {
        wprintw(window, "1) Insert file name\n");
        wprintw(window, "ESC) Quit\n");
        wrefresh(window);
        c = wgetch(window);
        switch (c)
        {
        case 0x31:
            curs_set(1);
            echo();
            wgetstr(window, buff);
            curs_set(0);
            path += buff;
            noecho();
            Sorter::GetInstance().Sort(path);
            wprintw(window, "Press any key to continue\n");
            wrefresh(window);
            getchar();
            c = 0x1B;
            break;
        default:
            wclear(window);
            break;
        }
    }
}

void App::GenerateChart() {}
