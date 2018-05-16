#pragma once

#include "curses_mem_vis.hpp"
#include "curses_bf_program_vis.hpp"
#include "curses_output_visualizer.hpp"

class curses_visualizer
{
public:
    curses_mem_vis mem_vis;
    curses_bf_program_vis code_vis;
    curses_output_vis output_vis;

    void init()
    {
        initscr();
        curs_set(0);
        start_color();
        init_pair(1, COLOR_YELLOW, COLOR_BLACK);

        auto rows = 32;

        printw("BrainF visualizer by Gediminas Masaitis");

        mem_vis.init(stdscr, 2, 0, rows);
        code_vis.init(stdscr, 2, mem_vis.width + 1);
        output_vis.init(stdscr, mem_vis.height + 2, 0);
    }

    ~curses_visualizer()
    {
        endwin();
    }
};
