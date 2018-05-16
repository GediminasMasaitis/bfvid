#pragma once

class curses_output_vis
{
    WINDOW* output_window;
    WINDOW* content_window;

public:
    const int height = 10;
    const int width = 150;

    void init(WINDOW* parent, const int y, const int x)
    {
        output_window = derwin(parent, height, width, y, x);
        content_window = derwin(output_window, height-2, width-2, 1, 1);

        box(output_window, 0, 0);

        wprintw(content_window, "Output:\n");

        wrefresh(output_window);
    }

    void print_ch(const char ch) const
    {
        wprintw(content_window, "%c", ch);
        wrefresh(content_window);
    }
};
