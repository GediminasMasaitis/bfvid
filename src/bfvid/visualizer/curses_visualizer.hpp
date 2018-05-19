#pragma once

#include "curses_mem_vis.hpp"
#include "curses_bf_program_vis.hpp"
#include "curses_output_visualizer.hpp"
#include "curses_vis_window.h"

class curses_visualizer
{
    curses_mem_vis mem_vis;
    curses_bf_program_vis prog_vis;
    curses_output_vis output_vis;

    curses_vis_window active_window;

private:
    void toggle_active_core(curses_vis_window window)
    {
        switch (window)
        {
        case curses_vis_window::none: break;
        case curses_vis_window::memory:
            mem_vis.toggle_active();
            break;
        case curses_vis_window::program:
            prog_vis.toggle_active();
            break;
        case curses_vis_window::output: break;
        default:;
        }
    }

public:
    void init()
    {
        initscr();
        curs_set(0);
        start_color();
        cbreak();
        noecho();
        init_pair(1, COLOR_YELLOW, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        keypad(stdscr, true);

        auto rows = 32;

        printw("bfvid by Gediminas Masaitis");

        mem_vis.init(stdscr, 2, 0, rows);
        prog_vis.init(stdscr, 2, mem_vis.width + 1);
        output_vis.init(stdscr, mem_vis.height + 2, 0);
        active_window = curses_vis_window::none;
    }

    void visualize(const std::string& program, const int8_t* memory, const size_t mem_len, const size_t mem_ptr = 0, const size_t instr_ptr = 0, const int steps = 0, const char in_ch = 0, const char out_ch = 0)
    {
        mem_vis.draw_memory(memory, mem_len, mem_ptr);
        prog_vis.set_program(program, instr_ptr);
        prog_vis.set_executed(steps);
        if (out_ch != 0)
        {
            output_vis.print_ch(out_ch);
        }
    }

    void left()
    {
        mem_vis.left();
    }

    void right()
    {
        mem_vis.right();
    }

    void up()
    {
        mem_vis.up();
    }

    void down()
    {
        mem_vis.down();
    }

    void toggle_active(curses_vis_window window)
    {
        if(window == active_window)
        {
            active_window = curses_vis_window::none;
        }
        else
        {
            toggle_active_core(active_window);
            active_window = window;
        }
        toggle_active_core(window);
    }

    ~curses_visualizer()
    {
        endwin();
    }
};
