#pragma once

#include "curses_mem_vis.hpp"
#include "curses_bf_program_vis.hpp"
#include "curses_output_visualizer.hpp"
#include "curses_vis_window.h"
#include "legend.h"
#include "trackbar.h"

class curses_visualizer
{
    curses_mem_vis mem_vis;
    curses_bf_program_vis prog_vis;
    curses_output_vis output_vis;
    legend leg;
    trackbar delay_trackbar;

    curses_vis_window active_window;

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
        init_pair(3, COLOR_RED, COLOR_BLACK);
        keypad(stdscr, true);

        auto rows = 16;

        //printw("bfvid by Gediminas Masaitis");

        mem_vis.init(stdscr, 0, 0, rows);
        prog_vis.init(stdscr, 0, mem_vis.width + 1, rows-4);
        output_vis.init(stdscr, mem_vis.height, 0);
        auto legend_y = mem_vis.height + output_vis.height;
        leg.init(stdscr, legend_y, 0);
        delay_trackbar.init(stdscr, legend_y, leg.width, 1, 1);
        active_window = curses_vis_window::none;
    }

    curses_vis_window get_active_window() const
    {
        return active_window;
    }

    size_t get_active_highlight() const
    {
        switch (active_window)
        {
        case curses_vis_window::program:
            return prog_vis.get_highlight();
        }
        return 0;
    }

    void visualize(const std::string& program, const int8_t* memory, const size_t mem_len, const breakpoint_mem_map& brs_mem, const breakpoint_instr_map& brs_instr, const size_t mem_ptr = 0, const size_t instr_ptr = 0, const int steps = 0, const char in_ch = 0, const char out_ch = 0)
    {
        mem_vis.draw_memory(memory, mem_len, mem_ptr);
        prog_vis.set_program(program, instr_ptr, brs_instr);
        prog_vis.set_executed(steps);
        if (out_ch != 0)
        {
            output_vis.print_ch(out_ch);
        }
    }

    void visualize(const brainfuck_interpreter<>& bf, const char in_ch = 0, const char out_ch = 0)
    {
        mem_vis.draw_memory(bf.memory.data(), bf.mem_size, bf.mem_ptr);
        prog_vis.set_program(bf.program, bf.instr_ptr, bf.breakpoints_instr);
        prog_vis.set_executed(bf.instr_steps);
        if (out_ch != 0)
        {
            output_vis.print_ch(out_ch);
        }
    }

    void set_delay(const int max, const int pos, const int value)
    {
        delay_trackbar.draw(max, pos, value);
    }

    void left()
    {
        mem_vis.left();
        prog_vis.left();
    }

    void right()
    {
        mem_vis.right();
        prog_vis.right();
    }

    void up()
    {
        mem_vis.up();
        prog_vis.up();
    }

    void down()
    {
        mem_vis.down();
        prog_vis.down();
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
        leg.draw(active_window);
    }

    ~curses_visualizer()
    {
        endwin();
    }
};
