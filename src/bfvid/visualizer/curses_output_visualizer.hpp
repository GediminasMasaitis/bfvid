#pragma once

class curses_output_vis : public window_base
{
    WINDOW* content_window;

public:
    const int height = 9;
    const int width = 150;

    curses_output_vis()
    {
        title = "[Output]";
        bold_first_title_letter = false;
    }

    void init(WINDOW* parent, const int y, const int x)
    {
        main_win = derwin(parent, height, width, y, x);
        content_window = derwin(main_win, height-2, width-2, 1, 1);

        draw_border();
    }

    void print_ch(const char ch) const
    {
        wprintw(content_window, "%c", ch);
        wrefresh(content_window);
    }
};
