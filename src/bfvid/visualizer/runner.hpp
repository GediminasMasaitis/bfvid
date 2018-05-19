#pragma once

#include "interpreter/brainfuck_interpreter.hpp"
#include "visualizer/curses_visualizer.hpp"

class runner
{
private:
    brainfuck_interpreter<>& bf;
    curses_visualizer& vis;
public:
    runner(brainfuck_interpreter<>& bf, curses_visualizer& vis) : bf(bf), vis(vis)
    {
        
    }

    void run(std::string program)
    {
        vis.visualize(bf.program, bf.memory.data(), bf.mem_size, bf.breakpoints_mem, bf.breakpoints_instr);
        bf.step_callback = [this](brainfuck_interpreter_core<>& bf2, char instruction, char in_ch, char out_ch)
        {
            vis.visualize(bf.program, bf.memory.data(), bf.mem_size, bf.breakpoints_mem, bf.breakpoints_instr, bf.mem_ptr, bf.instr_ptr, bf.instr_steps, in_ch, out_ch);
            //std::this_thread::sleep_for(std::chrono::milliseconds(10));
        };
        
        input_loop();
    }

    void input_loop()
    {
        while(true)
        {
            const auto ch = wgetch(stdscr);
            switch (ch)
            {
            case KEY_F(5):
                bf.run();
                break;
            case KEY_F(10):
                bf.step();
                break;
            case KEY_F(11):
                bf.step();
                break;

            case 'm':
            case 'M':
                vis.toggle_active(curses_vis_window::memory);
                break;

            case 'p':
            case 'P':
                vis.toggle_active(curses_vis_window::program);
                break;

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
            
            }
        }
    }
};
