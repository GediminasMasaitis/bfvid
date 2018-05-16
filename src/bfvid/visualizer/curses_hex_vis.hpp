#pragma once

class curses_hex_vis
{
    WINDOW* memory_window;

    WINDOW* side_legend_window;
    WINDOW* top_legend_window;
    WINDOW* ascii_legend_window;
    WINDOW* mem_hex_window;
    WINDOW* mem_ascii_window;

public:
    int width;
    int height;

    void init(int y, int x, int rows)
    {
        height = rows + 4;
        width = 82;

        auto cols = 16;
        //auto rows = (memory.size() + cols - 1) / cols;

        auto side_legend_window_height = rows;
        auto side_legend_window_width = 5;

        auto top_legend_window_height = 2;
        auto top_legend_window_width = 48;//mem_width - side_legend_window_width-4;

        auto right_windows_x = side_legend_window_width + top_legend_window_width + 10;
        auto bottom_windows_y = top_legend_window_height + 1;

        memory_window = newwin(height, width, y, x);
        side_legend_window = derwin(memory_window, side_legend_window_height, side_legend_window_width, 3, 2);
        top_legend_window = derwin(memory_window, top_legend_window_height, top_legend_window_width, 1, side_legend_window_width + 4);
        mem_hex_window = derwin(memory_window, side_legend_window_height, top_legend_window_width + 1, bottom_windows_y, side_legend_window_width + 4);
        mem_ascii_window = derwin(memory_window, side_legend_window_height, 17, bottom_windows_y, right_windows_x);
        ascii_legend_window = derwin(memory_window, top_legend_window_height, 17, 1, right_windows_x);
        refresh();

        box(side_legend_window, 0, 0);
        box(top_legend_window, 0, 0);
        //box(mem_hex_window, 0, 0);
        //box(mem_ascii_window, 0, 0);
        box(ascii_legend_window, 0, 0);
        box(memory_window, 0, 0);

        for (auto i = 0; i < side_legend_window_height; ++i)
        {
            mvwprintw(side_legend_window, i, 0, "%04X", i * 0x10);
        }

        for (auto i = 0; i < 16; ++i)
        {
            if (i == 8)
            {
                wprintw(top_legend_window, " ");
                wprintw(ascii_legend_window, " ");
            }
            wprintw(top_legend_window, "%1X", i);
            wprintw(ascii_legend_window, "%1X", i);
            if (i != 15)
            {
                wprintw(top_legend_window, "  ");
            }
        }
        wprintw(top_legend_window, " ");
        wrefresh(memory_window);
    }

    void draw_memory(int8_t* data, size_t length, size_t mem_ptr)
    {
        wmove(mem_hex_window, 0, 0);
        wmove(mem_ascii_window, 0, 0);
        for (auto i = 0; i < length; ++i)
        {
            if (i % 8 == 0 && i % 16 != 0)
            {
                wprintw(mem_hex_window, " ");
                wprintw(mem_ascii_window, " ");
            }
            if (i == mem_ptr)
            {
                wattron(mem_hex_window, COLOR_PAIR(1));
                wattron(mem_ascii_window, COLOR_PAIR(1));
            }
            auto ch = static_cast<uint8_t>(data[i]);
            wprintw(mem_hex_window, "%02X", ch);
            if (!isprint(ch))
            //if(ch == '\n')
            {
                ch = '.';
            }
            wprintw(mem_ascii_window, "%c", ch);
            if (i == mem_ptr)
            {
                wattroff(mem_hex_window, COLOR_PAIR(1));
                wattroff(mem_ascii_window, COLOR_PAIR(1));
            }
            wprintw(mem_hex_window, " ");
        }
        wrefresh(mem_hex_window);
        wrefresh(mem_ascii_window);
    }
};
