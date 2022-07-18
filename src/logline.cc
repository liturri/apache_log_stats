#include "logline.h"
#include "config.h"
#include <cstring>
#include <iostream>
#include <jpcre2.hpp>
#include <regex>

typedef jpcre2::select<char> jp;

const std::string reMatchString = R"(^([^ ]*) ([^ ]*) ([^ ]*) \[([^ ]*) ([^ ]*)\] \"([^ ]*) ([^ ]*) (HTTP\/[^ ]*)\" ([^ ]*) ([^ ]*) \"(.*[^ ]*)\" \"(.*)\" ([0-9]+)\/([0-9]+)$)";
const std::regex reMatchRegexp(reMatchString);
const jp::Regex re(reMatchString);

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

bool LogLine::ParseRegExp()
{
    std::smatch matchVector;
    if (!std::regex_match(line, matchVector, reMatchRegexp))
        return false;

    path = matchVector[7];
    source = matchVector[1];
    std::string timeString = matchVector[4];

    time_stamp = StrDate2Time(timeString);
    secs = std::stof(matchVector[14]) / 1000000.0;
    return true;
}

bool LogLine::Parse()
{
    if (cfg::GetStdReg())
        return ParseRegExp();
    else
        return ParsePcre2();
};

bool LogLine::ParsePcre2()
{

    jp::VecNum vec_num;
    jp::RegexMatch rm;
    size_t c = rm.setRegexObject(&re)                    // set associated Regex object
                   .setSubject(&line)                    // set subject string
                   .addModifier("")                      // add modifier
                   .setNumberedSubstringVector(&vec_num) // pass pointer to VecNum vector
                   .match();                             // Finally perform the match.
    if (c != 1)
        return false;

    const auto &matchVector = vec_num[0];

    // std::smatch matchVector;
    // if (!std::regex_match(line, matchVector, reMatchRegexp))
    //     return false;

    path = matchVector[7];
    source = matchVector[1];
    std::string timeString = matchVector[4];

    time_stamp = StrDate2Time(timeString);
    secs = std::stof(matchVector[14]) / 1000000.0;
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

std::string LogLine::NormalizedSource() const
{
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
