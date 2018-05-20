#pragma once

#include "interpreter/brainfuck_interpreter.hpp"
#include "visualizer/curses_visualizer.hpp"

class runner
{
private:
    brainfuck_interpreter<>& bf;
    curses_visualizer& vis;

    std::vector<int> delays{ 0,1,2,5,7,10,15,20,25,30,35,50,75,100,150,200,250,350,500,750,1000,1500,2000,3000 };
    int delay_idx;

public:
    runner(brainfuck_interpreter<>& bf, curses_visualizer& vis) : bf(bf), vis(vis)
    {
        delay_idx = 0;
    }

    void run(std::string program)
    {
        vis.visualize(bf.program, bf.memory.data(), bf.mem_size, bf.breakpoints_mem, bf.breakpoints_instr);
        bf.step_callback = [this](brainfuck_interpreter_core<>& bf2, char instruction, char in_ch, char out_ch)
        {
            vis.visualize(bf.program, bf.memory.data(), bf.mem_size, bf.breakpoints_mem, bf.breakpoints_instr, bf.mem_ptr, bf.instr_ptr, bf.instr_steps, in_ch, out_ch);
            //std::this_thread::sleep_for(std::chrono::milliseconds(10));
        };
        delay_idx = 5;
        apply_delay();
        input_loop();
    }

    void apply_delay()
    {
        if(delay_idx < 0)
        {
            delay_idx = 0;
        }
        if(delay_idx >= delays.size())
        {
            delay_idx = delays.size() - 1;
        }
        const auto delay = delays[delay_idx];
        vis.set_delay(delays.size(), delay_idx, delay);
        bf.delay = std::chrono::milliseconds(delay);
    }

    void input_loop()
    {
        while(true)
        {
            const auto ch = wgetch(stdscr);
            const auto active = vis.get_active_window();
            switch (ch)
            {

            case KEY_F(5):
                bf.run();
                break;

            case KEY_F(6):
                bf.reset();
                break;

            case KEY_F(10):
                bf.step_over();
                break;

            case KEY_F(11):
                bf.step();
                break;

            case KEY_F(12):
                bf.step_out();
                break;

            case 'm':
            case 'M':
                vis.toggle_active(curses_vis_window::memory);
                break;

            case 'p':
            case 'P':
                vis.toggle_active(curses_vis_window::program);
                break;

            case 'b':
            case 'B':
            {
                const auto br_pos = vis.get_active_highlight();
                switch (active)
                {
                case curses_vis_window::program:
                    bf.toggle_instr_break(br_pos);
                    break;
                }
                vis.visualize(bf);
                break;
            }

            case KEY_LEFT:
                vis.left();
                break;
            case KEY_RIGHT:
                vis.right();
                break;
            case KEY_UP:
                vis.up();
                break;
            case KEY_DOWN:
                vis.down();
                break;

            case KEY_HOME:
            {
                --delay_idx;
                apply_delay();
                break;
            }
            case KEY_END:
            {
                ++delay_idx;
                apply_delay();
                break;
            }
            }
        }
    }
};
