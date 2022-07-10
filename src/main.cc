#include "log_stats.h"
#include <iostream>
#include "term.h"
#include "app.h"


int main(int argc, char **argv)
{
    cfg::Init();
    std::string filename;
    if (argc != 2)
        filename = "/var/log/apache2/access.log";
    else
        filename = argv[1];
    StatApp app(filename);
    app.Run();
    return 0;
}