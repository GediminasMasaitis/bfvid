#pragma once

class curses_mem_vis
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

    void init(WINDOW* parent, int y, int x, int rows)
    {
        height = rows + 4;
        width = 82;
        auto cols = 16;

        // sl = side legend
        // tlh = top legend for hex
        // tla = top legend for ascii

        auto sl_height = rows;
        auto sl_width = 5;

        auto tlh_height = 2;
        auto tlh_width = 48;

        auto ascii_width = 17;
        auto tla_x = sl_width + tlh_width + 10;
        auto mem_ascii_y = tlh_height + 1;

        memory_window = derwin(parent, height, width, y, x);
        side_legend_window = derwin(memory_window, sl_height, sl_width, 3, 2);
        top_legend_window = derwin(memory_window, tlh_height, tlh_width, 1, sl_width + 4);
        mem_hex_window = derwin(memory_window, sl_height, tlh_width + 1, mem_ascii_y, sl_width + 4);
        mem_ascii_window = derwin(memory_window, sl_height, ascii_width, mem_ascii_y, tla_x);
        ascii_legend_window = derwin(memory_window, tlh_height, ascii_width, 1, tla_x);
        refresh();

        box(side_legend_window, 0, 0);
        box(top_legend_window, 0, 0);
        //box(mem_hex_window, 0, 0);
        //box(mem_ascii_window, 0, 0);
        box(ascii_legend_window, 0, 0);
        box(memory_window, 0, 0);

        for (auto i = 0; i < sl_height; ++i)
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

    void draw_memory(const int8_t* const data, const size_t length, const size_t mem_ptr) const
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
