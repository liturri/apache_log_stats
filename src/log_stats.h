#ifndef __LOG_STATS_H__
#define __LOG_STATS_H__
#include "config.h"
#include "follow.h"
#include <fstream>
#include <list>
#include <map>
#include <regex>
#include <string>
#include "summaries.h"
#include "logline.h"


class ParseLogFile
{
private:
    std::list<LogLine> urlList;
    std::map<std::string, PathSummary> counterPerPath;
    std::map<std::string, SourceSummary> counterPerSource;
    std::map<time_t, unsigned int> timeCouters;

    Follow logFile;

    long linesParsed = 0;

    // int itemsKeepTime;
    // int refreshTime;

public:
    ParseLogFile(const std::string &filename) : logFile(filename, 80000){};
    void CleanOldEntries();
    void Loop();
    const std::map<std::string, PathSummary> GetTopTenPath() const;
    const std::map<std::string, SourceSummary> GetTopTenSource() const;

    const std::map<time_t, unsigned int> &getTimeCouters() const { return timeCouters; }
};

#endif