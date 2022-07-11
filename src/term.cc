#include "term.h"

Term::Term() : Window(InitScreen())
{
}

Term::~Term()
{
}

WINDOW *Term::InitScreen()
{
    ::initscr();
    return stdscr;
}

void Term::Init()
{
    ::keypad(stdscr, TRUE);
    SetEcho(false);
    // Disable cursor   --   cursor::set_visibility(cursor::visibility::invisible);

    if (!::has_colors())
    {
        Write("Your terminal does not support color");
    }
    else
    {
        InitColor();
    }
    NoDelay(true);
}

void Term::Cleanup()
{
    ::endwin();
}

void Term::InitColor()
{
    ::start_color();
    ::use_default_colors();
    ::init_pair(1, COLOR_WHITE, COLOR_BLACK);
    ::init_pair(2, COLOR_BLACK, COLOR_RED);
    ::init_pair(3, COLOR_WHITE, COLOR_BLUE);
    ::init_pair(4, COLOR_BLACK, COLOR_YELLOW);
    ::init_pair(5, COLOR_BLUE, -1);
    ::init_pair(6, COLOR_BLACK, COLOR_MAGENTA);
    ::init_pair(7, COLOR_BLUE, COLOR_GREEN);
    ::init_pair(8, COLOR_YELLOW, -1);
    ::init_pair(9, COLOR_GREEN, -1);
}

void Window::SetColor(int color)
{
    ::wattron(thisWindow, COLOR_PAIR(color));
}

void Window::UnsetColor(int color)
{
    ::wattroff(thisWindow, COLOR_PAIR(color));
}

int Term::SetEcho(bool echo)
{
    if (echo)
    {
        return ::echo();
    }

    return ::noecho();
}

Window Term::NewWindow(int x1, int y1, int x2, int y2)
{
    return Window(x1, y1, x2, y2);
}

coord Window::Size() const
{
    coord c;
    getmaxyx(thisWindow, c.y, c.x);
    return c;
}

void Window::Refresh()
{
    ::wrefresh(thisWindow);
};

void Window::Border()
{
    ::wborder(thisWindow, 0, 0, 0, 0, 0, 0, 0, 0);
}

void Window::Erase()
{
    ::werase(thisWindow);
}

void Window::FillBg(chtype c)
{
    ::wbkgd(thisWindow, c);
}

int Window::GetKey()
{
    return wgetch(thisWindow);
};

void Window::NoDelay(bool d)
{
    ::nodelay(thisWindow, d);
}

Window::Window(int x1, int y1, int x2, int y2)
    : thisWindow(newwin(y2 - y1 + 1, x2 - x1 + 1, y1, x1)), deleteWindow(true)
{
}

Window::~Window()
{
    if (deleteWindow)
        ::delwin(thisWindow);
}

void Window::WriteXY(int x, int y, const std::string &str) const
{
    mvwaddstr(thisWindow, y, x, str.c_str());
}

void Window::WriteChar(chtype c)
{
    ::waddch(thisWindow, c);
}

void Window::Write(const std::string &str) const
{
    waddstr(thisWindow, str.c_str());
}