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


class Display
{
public:
  Display();
  void update(StateMachine machine);

private:
  int ch;
  std::string hint;
  cchar_t space;
  WINDOW *main_win, *state_win, *valves_win;
  std::chrono::time_point<std::chrono::system_clock> now, last;

  void reinitwin(WINDOW * win, int height, int width, int starty,int startx);
  void draw_state(StateMachine machine);
  void draw_colors();

};