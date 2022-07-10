#include <cstring>
#include <iostream>
#include "logline.h"


const std::regex reMatchRegexp(R"(^([^ ]*) ([^ ]*) ([^ ]*) \[([^ ]*) ([^ ]*)\] \"([^ ]*) ([^ ]*) (HTTP\/[^ ]*)\" ([^ ]*) ([^ ]*) \"(.*[^ ]*)\" \"(.*)\" ([0-9]+)\/([0-9]+)$)");


time_t StrDate2Time(std::string strDate)
{
    struct tm tm;
    time_t time_stamp;
    memset(&tm, 0, sizeof(tm));
    if (strptime(strDate.c_str(), "%d/%b/%Y:%H:%M:%S", &tm) == NULL)
    {
        std::cout << "Error parsing time: " << strDate << std::endl;
        exit(1);
    }
    tm.tm_isdst = 0;
    if ((time_stamp = mktime(&tm)) == -1)
    {
        std::cout << "Error parsing time: " << strDate << std::endl;
        exit(1);
    }
    return time_stamp;
}


bool LogLine::Parse()
{
    std::smatch sm;

    if (!std::regex_match(line, sm, reMatchRegexp))
    {
        return false;
    }
    path = sm[7];
    source = sm[1];

    std::string timeString = sm[4];
    time_stamp = StrDate2Time(timeString);
    secs = std::stof(sm[14]) / 1000000.0;
    return true;
}

std::string LogLine::NormalizedPath() const
{
    int pos = path.find("?");
    if (pos)
        return path.substr(0, pos);
    else
        return path;
}

std::string LogLine::NormalizedSource() const {
    return source;
}

std::string LogLine::GetMaxLenPath(unsigned int maxLength) const
{
    std::string normalPath = NormalizedPath();

    if (normalPath.length() > maxLength)
    {
        std::string aux = normalPath.substr(0, 3) + "...";
        aux += normalPath.substr(normalPath.length() - maxLength + 6);
        return aux;
    }
    else
    {
        return normalPath;
    }
}

