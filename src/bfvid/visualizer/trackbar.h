#pragma once

class trackbar
{
    WINDOW* main_win;

public:
    void init(WINDOW* parent, const int y, const int x, const int initial_pos, const int initial_value)
    {
        main_win = derwin(parent, 1, 40, y, x);
        //box(main_win, 0, 0);
        //draw(initial_pos, initial_value);
    }

    void draw(const int max, const int pos, const int value)
    {
        wclear(main_win);
        const auto left_ch = static_cast<char>(195);
        const auto bar_ch = static_cast<char>(196);
        const auto right_ch = static_cast<char>(180);
        const auto pos_ch = 'X';//static_cast<char>(215);
        std::string title = "Delay: ";
        mvwprintw(main_win, 0, 0, title.c_str());
        wprintw(main_win, "%c", left_ch);
        for(auto i = 0; i < max-2; ++i)
        {
            wprintw(main_win, "%c", bar_ch);
        }
        wprintw(main_win, "%c", right_ch);
        wprintw(main_win, "%6i", value);
        wprintw(main_win, " ms");
        mvwprintw(main_win, 0, title.size() + pos, "%c", pos_ch);
        wrefresh(main_win);
    }
};