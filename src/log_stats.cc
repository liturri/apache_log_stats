#include "log_stats.h"
#include "config.h"
#include <chrono>
#include <iomanip>

template <typename ContainerT, typename PredicateT>
void erase_if(ContainerT &items, const PredicateT &predicate)
{
    for (auto it = items.begin(); it != items.end();)
    {
        if (predicate(*it))
            it = items.erase(it);
        else
            ++it;
    }
}

void ParseLogFile::CleanOldEntries()
{
    const time_t maxOld = time(NULL) - cfg::GetItemsKeepTime();
    for (auto it = urlList.begin(); it != urlList.end();)
    {
        if (it->GetTime() < maxOld)
        {
            auto normalPath = it->NormalizedPath();
            auto normalSource = it->NormalizedSource();

            counterPerSource[normalSource] -= *it;
            if (counterPerSource[normalSource].GetCount() == 0)
                counterPerSource.erase(normalSource);

            counterPerPath[normalPath] -= *it;
            if (counterPerPath[normalPath].GetCount() == 0)
                counterPerPath.erase(normalPath);
            it = urlList.erase(it);
        }
        else
        {
            ++it;
        }
    }
    erase_if(timeCouters, [](const auto &a)
             { return a.first < time(NULL) - 10 * 60; });
}

void ParseLogFile::Loop()
{
    // Loop for 100 milliseconds
    auto timeNow = std::chrono::steady_clock::now();
    while (std::chrono::steady_clock::now() - timeNow < std::chrono::milliseconds(400))
    {
        std::string line = logFile.ReadLine();
        if (line == "")
            break;
        auto lline = LogLine(line);
        if (!lline.Parse())
            return;
        linesParsed++;
        urlList.push_back(lline);
        auto normalPath = lline.NormalizedPath();
        auto normalSource = lline.NormalizedSource();

        if (counterPerSource.find(normalSource) != counterPerSource.end())
            counterPerSource[normalSource] += lline;
        else
            counterPerSource[normalSource] = lline;

        if (counterPerPath.find(normalPath) != counterPerPath.end())
            counterPerPath[normalPath] += lline;
        else
            counterPerPath[normalPath] = lline;
        auto timeOfLine = lline.GetTime();
        if (timeOfLine >= time(NULL) - 10 * 60)
            timeCouters[timeOfLine]++;
    };
    if (lastCleanUp + std::chrono::seconds(1) < std::chrono::steady_clock::now())
    {
        lastCleanUp = std::chrono::steady_clock::now();
        CleanOldEntries();
    }

}

// int GetItemsKeepTime() { return itemsKeepTime; };
// void PrintStats();
const std::map<std::string, PathSummary> ParseLogFile::GetTopTenPath() const
{
    return counterPerPath;
};

const std::map<std::string, SourceSummary> ParseLogFile::GetTopTenSource() const
{
    return counterPerSource;
};
