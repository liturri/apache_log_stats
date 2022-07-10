#ifndef FOLLOW_H
#define FOLLOW_H
#include <string>
#include <fstream>
#include <thread>


class Follow
{
private:
    std::ifstream ifs;
    const std::string filename;

public:
    Follow(const std::string &filename, std::streamoff seekToEof = -1);
    ~Follow();
    std::string ReadLine();
    void SeekBackLines(std::streamoff linesBack);
};


#endif // FOLLOW_H
