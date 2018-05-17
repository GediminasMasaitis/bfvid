#pragma once

struct point
{
    int y = 0;
    int x = 0;
};

class curses_mem_vis
{
    WINDOW* memory_window;

    WINDOW* side_legend_window;
    WINDOW* top_legend_window;
    WINDOW* ascii_legend_window;
    WINDOW* mem_hex_window;
    WINDOW* mem_ascii_window;

    int highlighted_memory;
    size_t current_mem_ptr;

public:
    int width;
    int height;

    void init(WINDOW* parent, const int y, const int x, const int rows)
    {
        height = rows + 4;
        width = 82;
        auto cols = 16;

        // sl = side legend
        // tlh = top legend for hex
        // tla = top legend for ascii

        const auto sl_height = rows;
        const auto sl_width = 5;

        const auto tlh_height = 2;
        const auto tlh_width = 48;

        const auto ascii_width = 17;
        const auto tla_x = sl_width + tlh_width + 10;
        const auto mem_ascii_y = tlh_height + 1;

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

        mvwprintw(memory_window, 0, 10, "[Memory]");

        wrefresh(memory_window);
    }

    point get_hex_pt(const int mem_ptr)
    {
        point pt;
        pt.y = mem_ptr / 16;
        pt.x = (mem_ptr % 16) * 3;
        if (mem_ptr > 7)
        {
            ++pt.x;
        }
        return pt;
    }

    point get_ascii_pt(const int mem_ptr)
    {
        point pt;
        pt.y = mem_ptr / 8;
        pt.x = mem_ptr % 8;
        if (mem_ptr > 7)
        {
            ++pt.x;
        }
        return pt;
    }

    int get_color(const int mem_ptr)
    {
        return current_mem_ptr == mem_ptr ? 1 : 0;
    }

    void remove_memory_highlight()
    {
        if(highlighted_memory < 0)
        {
            return;
        }
        set_attr(highlighted_memory, A_NORMAL);
        highlighted_memory = -1;
    }

    void highlight_memory_cell(const int mem_ptr)
    {
        remove_memory_highlight();
        if (mem_ptr >= 0)
        {
            set_attr(mem_ptr, A_STANDOUT);
        }
        wrefresh(mem_hex_window);
        wrefresh(mem_ascii_window);
    }

    void set_attr(const int mem_ptr, const attr_t attr)
    {
        const auto hex_pt = get_hex_pt(mem_ptr);
        const auto ascii_pt = get_ascii_pt(mem_ptr);
        const auto color = get_color(mem_ptr);
        mvwchgat(mem_hex_window, hex_pt.y, hex_pt.x, 2, attr, color, nullptr);
        mvwchgat(mem_ascii_window, ascii_pt.y, ascii_pt.x, 1, attr, color, nullptr);
    }

    void draw_memory(const int8_t* const data, const size_t length, const size_t mem_ptr)
    {
        current_mem_ptr = mem_ptr;
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
                wattrset(mem_hex_window, COLOR_PAIR(1));
                wattrset(mem_ascii_window, COLOR_PAIR(1));
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
                wattrset(mem_hex_window, COLOR_PAIR(0));
                wattrset(mem_ascii_window, COLOR_PAIR(0));
            }
            wprintw(mem_hex_window, " ");
        }
        wrefresh(mem_hex_window);
        wrefresh(mem_ascii_window);
    }
};
