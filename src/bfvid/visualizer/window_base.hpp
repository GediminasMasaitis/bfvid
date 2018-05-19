#pragma once

class window_base
{
protected:
    std::string title;
    WINDOW* main_win;
    bool active;
    bool bold_first_title_letter;

    window_base()
    {
        active = false;
        bold_first_title_letter = true;
    }

    void draw_border() const
    {
        if (active)
        {
            wattrset(main_win, COLOR_PAIR(2));
        }
        box(main_win, 0, 0);
        mvwprintw(main_win, 0, 10, title.c_str());
        if (bold_first_title_letter)
        {
            mvwchgat(main_win, 0, 11, 1, A_BOLD, 2, nullptr);
        }
        if (active)
        {
            wattrset(main_win, COLOR_PAIR(0));
        }
        wrefresh(main_win);
    }

public:

    virtual void toggle_active()
    {
        set_active(!active);
    }

    virtual void set_active(bool a)
    {
        active = a;
        draw_border();
    }
};
