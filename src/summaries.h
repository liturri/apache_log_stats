#ifndef SUMMARIES_H
#define SUMMARIES_H

#include "config.h"
#include <string>
#include "logline.h"


class PathSummary
{
protected:
    int count;
    float sum_time;
    std::string normalizeKey;
    time_t oldestTime;

public:
    // Return count
    PathSummary() : count(0), sum_time(0), oldestTime(time(NULL)){};
    void operator-=(const LogLine &lline);

    PathSummary &operator+=(const LogLine &lline);
    virtual PathSummary &operator=(const LogLine &other);
    PathSummary &operator+=(const PathSummary &other);
    PathSummary &operator-=(const PathSummary &other);

    int GetCount() const { return count; };
    float GetCountPerSec() const { return static_cast<double>(count) / (time(NULL) - oldestTime); };
    float GetSumTime() const { return sum_time; };
    std::string GetNormalizeKey() const { return normalizeKey; }
    std::string GetMaxLenKey(unsigned int maxLength) const;
    time_t GetOldestTime() const { return std::max(oldestTime, time(NULL) - cfg::GetItemsKeepTime()); }

};

class SourceSummary : public PathSummary
{
public:
    SourceSummary &operator=(const LogLine &other) override;
};


#endif // SUMMARIES_H
