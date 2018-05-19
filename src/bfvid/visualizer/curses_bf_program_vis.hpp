#pragma once
#include "../interpreter/breakpoints.h"

class curses_bf_program_vis : public highlight_window
{
    WINDOW* code_window = nullptr;
    WINDOW* info_window = nullptr;

    const std::string executed_str = "Executed instructions: ";
    size_t current_instr_ptr;
    breakpoint_instr_map current_map;

public:
    const int height = 36;
    const int width = 67;
    const int code_width = 65;

    curses_bf_program_vis()
    {
        title = "[Program]";
        highlight_width = code_width;
    }

    void init(WINDOW* parent, const int y, const int x)
    {
        const auto code_height = 24;
        const auto info_y = code_height + 1;
        const auto info_height = 10;

        main_win = derwin(parent, height, width, y, x);
        code_window = derwin(main_win, code_height, code_width, 1, 1);
        info_window = derwin(main_win, info_height, code_width, info_y, 1);

        //box(code_window, 0, 0);
        //box(info_window, 0, 0);

        mvwprintw(info_window, 0, 0, executed_str.c_str());

        draw_border();
    }

    point get_pt(const int instr_ptr) const
    {
        point pt;
        pt.y = instr_ptr / code_width;
        pt.x = instr_ptr % code_width;
        return pt;
    }

    void remove_highlight() override
    {
        set_attr(highlight, highlight == current_instr_ptr ? A_BOLD : A_NORMAL);
    }

    void refresh_highlight_windows() override
    {
        wrefresh(code_window);
    }

    void set_highlight_core(const int instr_ptr) override
    {
        set_attr(instr_ptr, A_STANDOUT);
    }

    int get_color(const int instr_ptr)
    {
        if(current_map.find(instr_ptr) != current_map.end())
        {
            return  3;
        }
        return current_instr_ptr == instr_ptr ? 1 : 0;
    }

    void set_attr(const int instr_ptr, const attr_t attr)
    {
        const auto pt = get_pt(instr_ptr);
        const auto color = get_color(instr_ptr);
        mvwchgat(code_window, pt.y, pt.x, 1, attr, color, nullptr);
    }

    void set_program(const std::string& program, const int instr_ptr, const breakpoint_instr_map& breakpoints)
    {
        current_map = breakpoints;
        highlight_limit = program.size();
        current_instr_ptr = instr_ptr;
        wmove(code_window, 0, 0);
        for(auto i = 0; i < program.size(); ++i)
        {
            const auto ch = program[i];
            wprintw(code_window, "%c", ch);
        }

        if(instr_ptr == program.size())
        {
            wattron(info_window, COLOR_PAIR(1));
            mvwprintw(info_window, 2, 0, "Done!");
            wattroff(info_window, COLOR_PAIR(1));
            wrefresh(info_window);
        }
        else
        {
            const auto pt = get_pt(instr_ptr);
            mvwchgat(code_window, pt.y, pt.x, 1, A_BOLD, 1, nullptr);
        }

        for(const auto br_it : breakpoints)
        {
            const auto pt = get_pt(br_it.first);
            const auto attr = br_it.first == instr_ptr ? A_BOLD : A_NORMAL;
            mvwchgat(code_window, pt.y, pt.x, 1, attr, 3, nullptr);
        }
        set_highlight(highlight);
        wrefresh(code_window);
    }

    void set_executed(const int count) const
    {
        mvwprintw(info_window, 0, executed_str.size(), "%i", count);
        wrefresh(info_window);
    }
};
