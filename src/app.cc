#include "app.h"
#include <cmath>
#include <fmt/core.h>
#include <functional>
#include <vector>

StatApp::StatApp(const std::string &filename) : t(), parser(filename)
{
    setlocale(LC_ALL, "");
    t.Init();
}

StatApp::~StatApp()
{
    t.Cleanup();
}

void StatApp::Run()
{
    time_t timer = time(NULL) - 4;

    while (keepRunning)
    {
        parser.Loop();
        auto ch = t.GetKey();
        // if (ch == ERR)
        //     return;
        switch (ch)
        {
        case 'q':
            keepRunning = false;
            break;
        case ' ':
        case KEY_RESIZE:
            timer = 0;
            break;
        case 'd':
            displayMode = (displayMode + 1) % 2;
            timer = 0;
            break;
        case 's':
            // Next column sort
            sortColumn = (sortColumn + 1) % 5;
            timer = 0;
            break;
        case 'S':
            // Previous column sort
            sortColumn = (sortColumn + 4) % 5;
            timer = 0;
            break;
        case '?':
        case 'h':
            // Show help
            ShowHelp();
            timer = 0;
            break;
        case '.':
            // Write stats to file
            WriteStats();
            break;
        }
        if (time(NULL) - timer > cfg::GetRefreshTime())
        {
            timer = time(NULL);
            RefreshScreen();
        }
    };
}

void StatApp::RefreshScreen()
{
    std::vector<PathPair> topItemsList;
    std::function<bool(const PathPair &, const PathPair &)> sortCmp;
    // t.WriteXY(1, count + 2, fmt::format("{:<60}{:7d}{:8.2f}{:10.2f}{:10.2f}", item.second.GetMaxLenKey(60),
    // item.second.GetCount(), item.second.GetCountPerSec(), item.second.GetSumTime(), item.second.GetSumTime() / item.second.GetCount()));
    switch (sortColumn)
    {
    case 0:
        sortCmp = [](const PathPair &a, const PathPair &b)
        { return a.second.GetNormalizeKey() > b.second.GetNormalizeKey(); };
        break;
    case 1:
        sortCmp = [](const PathPair &a, const PathPair &b)
        { return a.second.GetCount() > b.second.GetCount(); };
        break;
    case 2:
        sortCmp = [](const PathPair &a, const PathPair &b)
        { return a.second.GetCountPerSec() > b.second.GetCountPerSec(); };
        break;
    case 3:
        sortCmp = [](const PathPair &a, const PathPair &b)
        { return a.second.GetSumTime() > b.second.GetSumTime(); };
        break;
    case 4:
        sortCmp = [](const PathPair &a, const PathPair &b)
        { return a.second.GetSumTime() / static_cast<float>(a.second.GetCount()) > b.second.GetSumTime() / static_cast<float>(b.second.GetCount()); };
        break;
    }
    // sortCmp = [](const PathPair &a, const PathPair &b)
    // { return a.second.GetSumTime() > b.second.GetSumTime(); };
    switch (displayMode)
    {
    case 0:
        for (auto &item : parser.GetTopTenSource())
            topItemsList.push_back(std::make_pair(item.first, item.second));
        break;
    case 1:
        for (auto &item : parser.GetTopTenPath())
            topItemsList.push_back(std::make_pair(item.first, item.second));
        break;
    }
    std::sort(topItemsList.begin(), topItemsList.end(), sortCmp);
    DisplayTopTen(topItemsList);
    DisplayStats();
    t.Refresh();
}

void StatApp::DisplayTopTen(const std::vector<PathPair> &topItemsList)
{
    int count = 0;
    int col = 0;
    t.SetColor(1);
    t.Erase();
    t.FillBg();
    t.Border();
    // ::box(stdscr, 0, 0);
    // auto &win = t.GetWin();
    t.SetColor(col++ == sortColumn ? 4 : 3);
    t.WriteXY(1, 2, fmt::format("{:^60}", ""));
    t.SetColor(col++ == sortColumn ? 4 : 3);
    t.Write(fmt::format("{:>7}", "Count"));
    t.SetColor(col++ == sortColumn ? 4 : 3);
    t.Write(fmt::format("{:>8}", "Count"));
    t.SetColor(col++ == sortColumn ? 4 : 3);
    t.Write(fmt::format("{:>10}", "Time"));
    t.SetColor(col++ == sortColumn ? 4 : 3);
    t.Write(fmt::format("{:>10}", "Time"));
    col = 0;
    t.SetColor(col++ == sortColumn ? 4 : 3);
    t.WriteXY(1, 3, fmt::format("{:^60}", "URL"));
    t.SetColor(col++ == sortColumn ? 4 : 3);
    t.Write(fmt::format("{:>7}", "Total"));
    t.SetColor(col++ == sortColumn ? 4 : 3);
    t.Write(fmt::format("{:>8}", "PerSec"));
    t.SetColor(col++ == sortColumn ? 4 : 3);
    t.Write(fmt::format("{:>10}", "Total"));
    t.SetColor(col++ == sortColumn ? 4 : 3);
    t.Write(fmt::format("{:>10}", "P/Call"));
    t.SetColor(5);
    for (auto item : topItemsList)
    {
        if (count > t.Size().y - 6)
            break;
        count++;
        t.WriteXY(1, count + 3, fmt::format("{:<60}{:7d}{:8.2f}{:10.2f}{:10.2f}   ", item.second.GetMaxLenKey(60), item.second.GetCount(), item.second.GetCountPerSec(), item.second.GetSumTime(), item.second.GetSumTime() / static_cast<float>(item.second.GetCount())));
    }
}

void StatApp::DisplayStats()
{
    auto &timeCounters = parser.getTimeCouters();
    int counter10 = 0, counter5 = 0, counter3 = 0, counter1 = 0;
    auto now = time(NULL);
    time_t minTime = time(NULL);
    for (auto &item : timeCounters)
    {
        minTime = std::min(minTime, item.first);
        if (now - item.first < 10 * 60)
            counter10 += item.second;
        if (now - item.first < 5 * 60)
            counter5 += item.second;
        if (now - item.first < 3 * 60)
            counter3 += item.second;
        if (now - item.first < 1 * 60)
            counter1 += item.second;
    }
    t.SetColor(6);
    t.WriteXY(1, 1, "Hits per seconds: ");
    // t.SetColor(5);
    minTime = time(NULL) - minTime;
    t.SetColor((minTime < static_cast<time_t>(1 * 60)) ? 5 : 7);
    t.Write(fmt::format(" 1: {:<0.2f}  ", (float)counter1 / (float)std::min(minTime, (time_t)(1 * 60))));
    t.SetColor((minTime < static_cast<time_t>(3 * 60)) ? 5 : 7);
    t.Write(fmt::format(" 3: {:<0.2f}  ", (float)counter3 / (float)std::min(minTime, (time_t)(3 * 60))));
    t.SetColor((minTime < static_cast<time_t>(5 * 60)) ? 5 : 7);
    t.Write(fmt::format(" 5: {:<0.2f}  ", (float)counter5 / (float)std::min(minTime, (time_t)(5 * 60))));
    t.SetColor((minTime < static_cast<time_t>(10 * 60)) ? 5 : 7);
    t.Write(fmt::format(" 10: {:<0.2f}  ", (float)counter10 / (float)std::min(minTime, (time_t)(10 * 60))));
}

void StatApp::ShowHelp()
{
    auto w = t.NewWindow(20, 20, 80, 35);
    w.Erase();
    w.FillBg();
    w.Border();
    w.Refresh();

    w.SetColor(A_UNDERLINE);
    w.SetColor(8);
    w.WriteXY(5, 2, "Apache access log realtime viewer");
    w.UnsetColor(A_UNDERLINE);
    w.WriteXY(5, 5, "Keyboard shortcuts");
    w.SetColor(9);
    w.WriteXY(5, 6, "  d    ");
    w.SetColor(8);
    w.Write("switch between url and source view");
    w.SetColor(9);
    w.WriteXY(5, 7, "  s    ");
    w.SetColor(8);
    w.Write("forward change sort column");
    w.SetColor(9);
    w.WriteXY(5, 8, "  S    ");
    w.SetColor(8);
    w.Write("backward change sort column");
    w.SetColor(9);
    w.WriteXY(5, 9, " SPACE ");
    w.SetColor(8);
    w.Write("refresh display");
    w.SetColor(9);
    w.WriteXY(5, 10, "  q   ");
    w.SetColor(8);
    w.Write("quit");

    w.NoDelay(false);
    w.GetKey();
    w.NoDelay(true);
}

void StatApp::WriteStats()
{
    // Write timecounter stats to filename
    std::ofstream file("/tmp/timecounter.dump", std::ios::out | std::ios::trunc);
    if (file.is_open())
    {
        file << "Time: from " << time(NULL) - 10 * 60 << " to " << time(NULL) << std::endl;
        for (auto &item : parser.getTimeCouters())
        {
            file << item.first << " " << item.second << std::endl;
        }
        file.close();
    }
}
