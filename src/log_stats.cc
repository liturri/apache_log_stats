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
    time_t now = time(NULL);
    urlList.sort([](const auto &a, const auto &b) -> bool
                 { return a.GetTime() < b.GetTime(); });
    while (urlList.size() > 0 && urlList.begin()->GetTime() < now - cfg::GetItemsKeepTime())
    {
        auto item = urlList.begin();
        auto normalPath = item->NormalizedPath();
        auto normalSource = item->NormalizedSource();

        counterPerSource[normalSource] -= *item;
        if (counterPerSource[normalSource].GetCount() == 0)
            counterPerSource.erase(normalSource);

        counterPerPath[normalPath] -= *item;
        if (counterPerPath[normalPath].GetCount() == 0)
            counterPerPath.erase(normalPath);
        urlList.pop_front();
    }
    erase_if(timeCouters, [](const auto &a)
             { return a.first < time(NULL) - 10 * 60; });
}

void ParseLogFile::Loop()
{
    // Loop for 100 milliseconds
    auto timeNow = std::chrono::system_clock::now();
    while (std::chrono::system_clock::now() - timeNow < std::chrono::milliseconds(400))
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
        if (lline.GetTime() >= time(NULL) - 10 * 60)
            timeCouters[lline.GetTime()]++;
    };
    CleanOldEntries();
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
