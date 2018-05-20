#pragma once
#include "curses_vis_window.h"

class legend
{
    struct legend_entry
    {
        std::string key;
        std::string title;

        legend_entry(const std::string& key, const std::string& title)
            : key(key),
              title(title)
        {
        }
    };

    WINDOW* main_win;

    std::vector<legend_entry> common_items
    {
        legend_entry("F5 ", "Run program"),
        legend_entry("F10", "Step over"),
        legend_entry("F11", "Step into"),
        legend_entry("F12", "Step out"),
    };

    std::vector<legend_entry> mem_instr_items
    {
        legend_entry("B  ", "Toggle breakpt"),
    };

    const int title_width = 25;
    const int item_width = 15;

    void draw_items(const std::vector<legend_entry>& items, const int current_offs) const
    {
        const auto offs = title_width + current_offs * item_width;
        for (size_t i = 0; i < items.size(); ++i)
        {
            auto& item = items[i];
            wattrset(main_win, COLOR_PAIR(2) | A_REVERSE);
            mvwprintw(main_win, 0, offs + i * item_width, item.key.c_str());
            wattrset(main_win, COLOR_PAIR(0));
            wprintw(main_win, item.title.c_str());
        }
    }

    void draw_title(curses_vis_window active_win) const
    {
        wattrset(main_win, A_BOLD);
        mvwprintw(main_win, 0, 0, "Active window: ");
        std::string title;
        //const auto color = active_win == curses_vis_window::none ? 0 : 2;
        //wattrset(main_win, COLOR_PAIR(color) | A_BOLD);
        switch (active_win)
        {
        case curses_vis_window::none:
            title = "None";
            break;
        case curses_vis_window::memory:
            title = "Memory";
            break;
        case curses_vis_window::program:
            title = "Program";
            break;
        case curses_vis_window::output: break;
        default:;
        }
        wprintw(main_win, title.c_str());
    }

public:

    const int height = 1;
    const int width = 110;

    void init(WINDOW* parent, int y, int x)
    {
        main_win = derwin(parent, height, width, y, x);
        //box(main_win, 0, 0);
        draw(curses_vis_window::none);
    }

    void draw(const curses_vis_window active_win) const
    {
        wclear(main_win);

        draw_title(active_win);

        draw_items(common_items, 0);
        if (active_win == curses_vis_window::memory || active_win == curses_vis_window::program)
        {
            draw_items(mem_instr_items, common_items.size());
        }

        wrefresh(main_win);
    }
};
