#include "config.h"

int cfg::refreshTime;
int cfg::itemsKeepTime;

void cfg::Init()
{
    refreshTime = 5;
    itemsKeepTime = 600;
}
