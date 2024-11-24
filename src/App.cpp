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

    outputFile.open("./output/experiment.txt");

    RunTestOnTape("./tapes/test.txt", 10);
    RunTestOnTape("./tapes/test2.txt", 100);
    RunTestOnTape("./tapes/test3.txt", 500);
    RunTestOnTape("./tapes/test4.txt", 1000);
    RunTestOnTape("./tapes/test5.txt", 2000);
    RunTestOnTape("./tapes/test6.txt", 5000);
    RunTestOnTape("./tapes/test7.txt", 10000);
    RunTestOnTape("./tapes/test8.txt", 20000);
    RunTestOnTape("./tapes/test9.txt", 50000);
    RunTestOnTape("./tapes/test10.txt", 100000);

    wprintw(window, "Press any key to continue\n");
    wrefresh(window);
    getchar();

    outputFile.close();

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
        wprintw(window, "e) Experiment\n");
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
        case 0x65:
            Experiment();
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

void App::RunTestOnTape(std::string filename, size_t N)
{
    wprintw(window, "N = %ld\n", N);
    wrefresh(window);
    Sorter::GetInstance().Sort(filename, true);
    wprintw(window, "%ld %ld\n", Sorter::GetInstance().GetInitialNumberOfRuns(), Sorter::GetInstance().GetNumberOfRecords());
    wrefresh(window);

    outputFile << Sorter::GetInstance().GetNumberOfRecords() << " " << Sorter::GetInstance().GetInitialNumberOfRuns() << " "
               << Sorter::GetInstance().GetNumberOfPhases() << " " << Sorter::GetInstance().GetNumberOfFileAccesses() << " "
               << Sorter::GetInstance().GetTheoreticalNumberoOfPhases() << " " << Sorter::GetInstance().GetTheoreticalFileAccesses() << std::endl;
}