#include "app.h"
#include "config.h"
#include "log_stats.h"
#include "term.h"
#include <argparse/argparse.hpp>
#include <iostream>

void argParse(int argc, char **argv)
{
    argparse::ArgumentParser program("apache_log_stats");
    program.add_argument("--log-file", "-l")
        .help("Path to log file")
        .default_value(std::string("/var/log/apache2/access.log"))
        .required();
    program.add_argument("--stdreg", "-s")
        .help("Use std:regexp lib instead of pcre2")
        .default_value(false)
        .implicit_value(true);
    program.add_argument("--refresh-time", "-r")
        .help("Refresh time in seconds")
        .default_value(5);

    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error &err)
    {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    cfg::SetStdReg(program.get<bool>("--stdreg"));
    cfg::SetLogfileName(program.get<std::string>("--log-file"));
    cfg::SetRefreshTime(program.get<int>("--refresh-time"));
}

int main(int argc, char **argv)
{
    cfg::Init();
    argParse(argc, argv);
    StatApp app(cfg::GetLogfileName());
    app.Run();
    return 0;
}