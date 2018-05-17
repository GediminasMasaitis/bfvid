#pragma once

#include "curses_mem_vis.hpp"
#include "curses_bf_program_vis.hpp"
#include "curses_output_visualizer.hpp"

class curses_visualizer
{
    curses_mem_vis mem_vis;
    curses_bf_program_vis code_vis;
    curses_output_vis output_vis;

public:
    void init()
    {
        initscr();
        curs_set(0);
        start_color();
        init_pair(1, COLOR_YELLOW, COLOR_BLACK);

        auto rows = 32;

        printw("bfvid by Gediminas Masaitis");

        mem_vis.init(stdscr, 2, 0, rows);
        code_vis.init(stdscr, 2, mem_vis.width + 1);
        output_vis.init(stdscr, mem_vis.height + 2, 0);
    }

    void visualize(const std::string& program, const int8_t* memory, const size_t mem_len, const size_t mem_ptr = 0, const size_t instr_ptr = 0, const int steps = 0, const char in_ch = 0, const char out_ch = 0)
    {
        mem_vis.draw_memory(memory, mem_len, mem_ptr);
        code_vis.set_program(program, instr_ptr);
        code_vis.set_executed(steps);
        if (out_ch != 0)
        {
            output_vis.print_ch(out_ch);
        }
        mem_vis.highlight_memory_cell(0);
    }

    ~curses_visualizer()
    {
        endwin();
    }
};
