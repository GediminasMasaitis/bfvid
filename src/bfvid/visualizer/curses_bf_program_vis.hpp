#pragma once

class curses_bf_program_vis : public window_base
{
    WINDOW* code_window = nullptr;
    WINDOW* info_window = nullptr;

    const std::string executed_str = "Executed instructions: ";

public:
    const int height = 36;
    const int width = 67;

    curses_bf_program_vis()
    {
        title = "[Program]";
    }

    void init(WINDOW* parent, const int y, const int x)
    {
        const auto code_height = 24;
        const auto info_y = code_height + 1;
        const auto info_height = 10;

        main_win = derwin(parent, height, width, y, x);
        code_window = derwin(main_win, code_height, width - 2, 1, 1);
        info_window = derwin(main_win, info_height, width - 2, info_y, 1);

        //box(code_window, 0, 0);
        //box(info_window, 0, 0);

        mvwprintw(info_window, 0, 0, executed_str.c_str());

        draw_border();
    }

    void set_program(const std::string& program, const int instr_ptr) const
    {
        wmove(code_window, 0, 0);
        for(auto i = 0; i < program.size(); ++i)
        {
            if (i == instr_ptr)
            {
                wattron(code_window, COLOR_PAIR(1));
            }
            const auto ch = program[i];
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

    void set_executed(const int count) const
    {
        mvwprintw(info_window, 0, executed_str.size(), "%i", count);
        wrefresh(info_window);
    }
};
