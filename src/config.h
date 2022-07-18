#ifndef CONFIG_H
#define CONFIG_H
#include <string>

class cfg
{
private:
    static int refreshTime;
    static int itemsKeepTime;
    static bool stdregexp;
    static std::string logfileName;

public:
    static void Init();

    static int GetItemsKeepTime() { return cfg::itemsKeepTime; }
    static void SetItemsKeepTime(int itemsKeepTime) { cfg::itemsKeepTime = itemsKeepTime; }

    static int GetRefreshTime() { return cfg::refreshTime; }
    static void SetRefreshTime(int refreshTime) { cfg::refreshTime = refreshTime; }

    static bool GetStdReg() { return cfg::stdregexp; }
    static void SetStdReg(bool stdreg) { cfg::stdregexp = stdreg; }

    static const std::string &GetLogfileName() { return cfg::logfileName; }
    static void SetLogfileName(const std::string &filename) { cfg::logfileName = filename; }
};

#endif // CONFIG_H
