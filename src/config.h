#ifndef CONFIG_H
#define CONFIG_H

class cfg
{
private:
    static int refreshTime;
    static int itemsKeepTime;

public:
    static void Init();

    static int GetItemsKeepTime() { return cfg::itemsKeepTime; }
    static void SetItemsKeepTime(int itemsKeepTime) { cfg::itemsKeepTime = itemsKeepTime; }

    static int GetRefreshTime() { return cfg::refreshTime; }
    static void SetRefreshTime(int refreshTime) { cfg::refreshTime = refreshTime; }

};

#endif // CONFIG_H
