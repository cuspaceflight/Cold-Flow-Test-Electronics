#pragma once

#include <cursesw.h>
#include <iostream>

#if __has_include(<ncursesw/curses.h>)
#include <ncursesw/curses.h>
#else

#include <ncurses.h>

#endif

#include <string.h>
#include<chrono>

#include "State.hpp"
#include "Relay.hpp"


class Display {
public:
    Display(StateMachine *statemachine, Relay *relays);

    void update();
    bool open;

private:
    int ch;
    std::string hint;
    cchar_t space;
    WINDOW *main_win, *state_win, *valves_win;
    StateMachine *machine;
    Relay *relays;
    std::chrono::time_point<std::chrono::system_clock> now, last;

    void reinitwin(WINDOW *win, int height, int width, int starty, int startx);

    void draw_state();

    void draw_colors();

};
