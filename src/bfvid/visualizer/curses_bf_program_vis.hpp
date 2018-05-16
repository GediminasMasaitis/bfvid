#pragma once
#include <curses/curses.h>

class curses_bf_program_vis
{
    WINDOW* program_window;
    WINDOW* code_window;
    WINDOW* info_window;
    
public:
    void init(int y, int x)
    {
        auto height = 36;
        auto width = 67;

        auto code_height = 24;
        auto info_y = code_height + 1;
        auto info_height = 10;


        program_window = newwin(height, width, y, x);
        code_window = derwin(program_window, code_height, width - 2, 1, 1);
        info_window = derwin(program_window, info_height, width - 2, info_y, 1);

        box(program_window, 0, 0);
        //box(code_window, 0, 0);
        //box(info_window, 0, 0);

        mvwprintw(info_window, 0, 0, "Executed instructions: 0");

        wrefresh(program_window);
    }

    void set_program(std::string program, int instr_ptr)
    {
        wmove(code_window, 0, 0);
        for(auto i = 0; i < program.size(); ++i)
        {
            if (i == instr_ptr)
            {
                wattron(code_window, COLOR_PAIR(1));
            }
            auto ch = program[i];
            wprintw(code_window, "%c", ch);
            if (i == instr_ptr)
            {
                wattroff(code_window, COLOR_PAIR(1));
            }
        }

        if(instr_ptr == program.size())
        {
            wattron(info_window, COLOR_PAIR(1));
            mvwprintw(info_window, 2, 0, "Done!");
            wattroff(info_window, COLOR_PAIR(1));
            wrefresh(info_window);
        }
        
        wrefresh(code_window);
    }

    void set_executed(int count)
    {
        mvwprintw(info_window, 0, 23, "%i", count);
        wrefresh(info_window);
    }
};
