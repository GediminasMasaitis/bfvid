// Brainfuck.cpp : Defines the entry point for the console application.
//

#include "stdafx.hpp"

#include "interpreter/brainfuck_interpreter.hpp"
#include "visualizer/curses_visualizer.hpp"

#include <iostream>
#include <sstream>
#include <thread>

using namespace std;

int main()
{
    string program = "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>."; // Hello world
    //string program = ",>,<[>>+>+<<<-]>>>[<<<+>>>-]<<[>+>+<<-]>>[<<+>>-]>."; // My addition
    //string program = "+[.+]";
    //string input = "\11a";
    //stringstream in_ss(input);
    stringstream in_ss("\x15\x1e");
    //in_ss << (char)5 << (char)3;
    stringstream out_ss;
    brainfuck_interpreter<> bf(program, &out_ss, &in_ss);

    curses_visualizer visualizer;
    visualizer.init();
    auto counter = 0;
    bf.step_callback = [&visualizer, &counter](brainfuck_interpreter<>& bf, char instruction, char in_ch, char out_ch)
    {
        visualizer.hex_vis.draw_memory(bf.memory.data(), bf.memory.size(), bf.mem_ptr);
        visualizer.code_vis.set_program(bf.program, bf.instr_ptr);
        visualizer.code_vis.set_executed(++counter);
        if(out_ch != 0)
        {
            visualizer.output_vis.print_ch(out_ch);
        }
        std::this_thread::sleep_for(10ms);
    };

    getc(stdin);

    bf.run();

    getc(stdin);
    return 0;
}