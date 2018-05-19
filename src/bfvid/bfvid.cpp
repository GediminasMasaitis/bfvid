// Brainfuck.cpp : Defines the entry point for the console application.
//

#include "stdafx.hpp"

#include "interpreter/brainfuck_interpreter.hpp"
#include "visualizer/runner.hpp"

using namespace std;

int main()
{
    std::string program = "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>."; // Hello world
    std::stringstream in_ss("\x15\x1e");
    std::stringstream out_ss;
    brainfuck_interpreter<> bf(program, &out_ss, &in_ss);
    bf.set_instr_break(42);
    curses_visualizer vis;
    vis.init();
    runner runner(bf, vis);
    runner.run(program);

    getc(stdin);
    return 0;
}