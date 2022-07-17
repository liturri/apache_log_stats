#ifndef __LOG_STATS_H__
#define __LOG_STATS_H__
#include "config.h"
#include "follow.h"
#include "logline.h"
#include "summaries.h"
#include <fstream>
#include <list>
#include <map>
#include <string>

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

protected:
    void CleanOldEntries();

public:
    ParseLogFile(const std::string &filename) : logFile(filename, 80000){};
    void Loop();
    const std::map<std::string, PathSummary> GetTopTenPath() const;
    const std::map<std::string, SourceSummary> GetTopTenSource() const;

    const std::map<time_t, unsigned int> &getTimeCouters() const { return timeCouters; }
};

#endif