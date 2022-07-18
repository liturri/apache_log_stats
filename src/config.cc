#include "config.h"

int cfg::refreshTime;
int cfg::itemsKeepTime;
bool cfg::stdregexp;
std::string cfg::logfileName;

void cfg::Init()
{
    refreshTime = 5;
    itemsKeepTime = 600;
    stdregexp = true;
    logfileName = "/var/log/apache2/access.log";
}
