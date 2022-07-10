#include "summaries.h"
std::string PathSummary::GetMaxLenKey(unsigned int maxLength) const
{
    if (normalizeKey.length() > maxLength)
    {
        std::string aux = normalizeKey.substr(0, 3) + "...";
        aux += normalizeKey.substr(normalizeKey.length() - maxLength + 6);
        return aux;
    }
    else
    {
        return normalizeKey;
    }
}

// operator -=
void PathSummary::operator-=(const LogLine &lline)
{
    count--;
    sum_time -= lline.GetSecs();
    oldestTime = time(NULL) - cfg::GetItemsKeepTime();
};

// operator +=
PathSummary &PathSummary::operator+=(const LogLine &lline)
{
    count++;
    sum_time += lline.GetSecs();
    oldestTime = std::min(oldestTime, lline.GetTime());
    return *this;
}

// operator =
PathSummary &PathSummary::operator=(const LogLine &other)
{
    count = 1;
    sum_time = other.GetSecs();
    normalizeKey = other.NormalizedPath();
    oldestTime = other.GetTime();
    return *this;
}

// operator =
SourceSummary &SourceSummary::operator=(const LogLine &other)
{
    count = 1;
    sum_time = other.GetSecs();
    normalizeKey = other.NormalizedSource();
    oldestTime = other.GetTime();
    return *this;
}

// operator add
PathSummary &PathSummary::operator+=(const PathSummary &other)
{
    count += other.count;
    sum_time += other.sum_time;
    oldestTime = std::min(oldestTime, other.oldestTime);
    return *this;
}

// operator subtract
PathSummary &PathSummary::operator-=(const PathSummary &other)
{
    count -= other.count;
    sum_time -= other.sum_time;
    oldestTime = std::min(oldestTime, other.oldestTime);
    return *this;
}
