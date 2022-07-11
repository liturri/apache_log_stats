#ifndef TERM_H
#define TERM_H

#include <memory>
#include <ncurses.h>
#include <string>

class coord
{
public:
    coord() : x(0), y(0){};
    coord(int x, int y) : x(x), y(y) {}
    int x;
    int y;
};

class Window
{
private:
    WINDOW *thisWindow;
    bool deleteWindow;

public:
    Window(int x1, int y1, int x2, int y2);
    Window(WINDOW *window) : thisWindow(window), deleteWindow(false){};
    ~Window();
    int SetEcho(bool echo);
    void WriteXY(int x, int y, const std::string &str) const;
    void WriteYX(int y, int x, const std::string &str) const { WriteXY(x, y, str); };
    void WriteChar(chtype c);
    void Write(const std::string &str) const;
    void SetColor(int color);
    void UnsetColor(int color);
    coord Size() const;
    void Refresh();
    void Border();
    void Erase();
    void FillBg(chtype c = ' ');
    int GetKey();
    void NoDelay(bool d);
};

class Term : public Window
{

public:
    Term();
    ~Term();
    static WINDOW *InitScreen();
    void Init();
    void Cleanup();
    void InitColor();
    int SetEcho(bool echo);
    // void WriteXY(int x, int y, const std::string &str) const;
    // void WriteYX(int y, int x, const std::string &str) const { WriteXY(x, y, str); };
    // void Write(const std::string &str) const;
    // coord Size() const;
    // void Refresh();
    Window NewWindow(int x1, int y1, int x2, int y2);
    Term(const Term &) = delete;
    Term(Term &&) = delete;
    Term &operator=(const Term &) = delete;
    Term &operator=(Term &&) = delete;
};

#endif // TERM_H
