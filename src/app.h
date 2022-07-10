#ifndef APP_H
#define APP_H
#include "config.h"
#include "log_stats.h"
#include "term.h"
#include <string>

typedef std::pair<std::string, PathSummary> PathPair;


class StatApp
{
public:
    StatApp(const std::string &filename);
    ~StatApp();
    void Run();
    void RefreshScreen();

    void DisplayTopTen(const std::vector<PathPair> &topItemsList);
    void DisplayStats();

    void ShowHelp();

    void WriteStats();

private:
    Term t;
    ParseLogFile parser;
    bool keepRunning = true;
    int displayMode = 1;
    int sortColumn = 3;
};

#endif // APP_H
