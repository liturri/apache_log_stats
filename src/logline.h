#ifndef LOGLINE_H
#define LOGLINE_H

#include <regex>
#include <string>

class LogLine
{
private:
    const std::string line;
    float secs;
    std::string path;
    std::string source;
    time_t time_stamp;

public:
    LogLine(const std::string &line) : line(line){};
    bool Parse();
    std::string NormalizedPath() const;
    std::string NormalizedSource() const;

    std::string GetMaxLenPath(unsigned int maxLength) const;
    time_t GetTime() const { return time_stamp; };
    float GetSecs() const { return secs; };
};

#endif // LOGLINE_H
