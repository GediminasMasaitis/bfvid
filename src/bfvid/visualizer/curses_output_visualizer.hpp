#pragma once

class curses_output_vis
{
    WINDOW* output_window;
    WINDOW* content_window;

public:
    void init(int y, int x)
    {
        auto height = 10;
        auto width = 150;
        output_window = newwin(height, width, y, x);
        content_window = derwin(output_window, height-2, width-2, 1, 1);

        box(output_window, 0, 0);

        wprintw(content_window, "Output:\n");

        wrefresh(output_window);
    }

    void print_ch(int8_t ch)
    {
        wprintw(content_window, "%c", ch);
        wrefresh(content_window);
    }
};
