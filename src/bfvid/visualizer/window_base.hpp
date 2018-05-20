#pragma once

struct point
{
    int y = 0;
    int x = 0;
};

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

class highlight_window : public window_base
{
protected:
    size_t highlight;
    size_t highlight_width;
    size_t highlight_limit;

    virtual void refresh_highlight_windows() = 0;
    virtual void set_highlight_core(const int ptr) = 0;
    virtual void remove_highlight() = 0;

    void set_highlight(const size_t ptr)
    {
        if (!active)
        {
            return;
        }
        if (ptr < 0 || ptr > highlight_limit)
        {
            return;
        }
        remove_highlight();
        highlight = ptr;
        set_highlight_core(ptr);
        refresh_highlight_windows();
    }

public:

    highlight_window() : highlight(0)
    {
        
    }

    size_t get_highlight() const
    {
        return highlight;
    }

    void set_active(bool a) override
    {
        window_base::set_active(a);
        if (a)
        {
            set_highlight(highlight);
        }
        else
        {
            remove_highlight();
        }
        refresh_highlight_windows();
    }

    void left()
    {
        set_highlight(highlight - 1);
    }

    void right()
    {
        set_highlight(highlight + 1);
    }

    void up()
    {
        set_highlight(highlight - highlight_width);
    }

    void down()
    {
        set_highlight(highlight + highlight_width);
    }
};
