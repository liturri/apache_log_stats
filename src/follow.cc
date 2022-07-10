#include "follow.h"
#include <algorithm>
#include <iostream>
#include <vector>

Follow::Follow(const std::string &filename, std::streamoff seekToEof) : filename(filename)
{
    ifs.open(filename);
    if (!ifs.is_open())
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(1);
    }
    if (seekToEof == -1)
        ifs.seekg(0, std::ios::end);
    else if (seekToEof == 0)
        ifs.seekg(0, std::ios::beg);
    else if (seekToEof > 0)
        SeekBackLines(seekToEof);
}

Follow::~Follow()
{
    ifs.close();
}

std::string Follow::ReadLine()
{
    std::string line;
    while (true)
    {
        std::getline(ifs, line);
        if (ifs.eof())
        {
            ifs.clear();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            // sleep(1);
            return "";
            // continue;
        }
        if (line == "")
            continue;
        return line;
    }
}

// Buffered read of lines from the file.
void Follow::SeekBackLines(std::streamoff linesBack)
{
    std::size_t bufferSize = 4096;
    std::vector<u_int8_t> buffer(bufferSize);

    // std::streamoff count = 0;
    ifs.seekg(0, std::ios::end);
    std::streamoff fileSize = ifs.tellg();
    std::streamoff pos = fileSize;
    std::streamoff blockStart = fileSize;

    while (pos > 0 && linesBack > 0)
    {
        if (pos <= blockStart)
        {
            if (bufferSize > static_cast<size_t>(blockStart))
                bufferSize = blockStart;
            blockStart -= bufferSize;
            ifs.seekg(blockStart, std::ios::beg);
            ifs.read(reinterpret_cast<char *>(buffer.data()), bufferSize);
        }
        auto ch = buffer[(pos--) - blockStart];
        if (ch == '\n')
            linesBack--;
    }
    ifs.seekg(pos+1, std::ios::beg);
}

// void Follow::SeekBackLines(std::streamoff linesBack)
// {
//     ifs.seekg(0, std::ios::end);
//     std::streamoff count = 0;
//     while (ifs.tellg() > 0 && count < linesBack)
//     {
//         ifs.seekg(-1, std::ios::cur);
//         auto ch = ifs.peek();
//         if (ch == '\n')
//         {
//             count++;
//         }
//     }
//     ifs.peek();
// }
