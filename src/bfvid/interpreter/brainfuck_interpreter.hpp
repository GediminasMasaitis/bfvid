#pragma once

#include "brainfuck_interpreter_core.h"
#include "breakpoints.h"

template<typename TData = int8_t, size_t VMemSize = 256>
class brainfuck_interpreter : public brainfuck_interpreter_core<TData, VMemSize>
{
public:

    breakpoint_mem_map breakpoints_mem;
    breakpoint_instr_map breakpoints_instr;
    bool breakpoint_loop_set;
    size_t breakpoint_loop;

    brainfuck_interpreter(const std::string& program_str, std::ostream* const out = nullptr, std::istream* const in = nullptr) 
    : brainfuck_interpreter_core<TData, VMemSize>(program_str, out, in),
      breakpoint_loop_set(false),
      breakpoint_loop(0)
    {
        
    }

    void set_instr_break(const size_t ptr)
    {
        auto br = breakpoint_instr(breakpoint_instr_type::execute);
        set_instr_break(ptr, br);
    }

    void set_instr_break(const size_t ptr, const breakpoint_instr& br)
    {
        breakpoints_instr[ptr] = br;
    }

    void run()
    {
        while (this->step())
        {
            if(breakpoint_loop_set && this->instr_ptr == breakpoint_loop)
            {
                break;
            }

            auto br_instr_it = breakpoints_instr.find(this->instr_ptr);
            if(br_instr_it != breakpoints_instr.end())
            {
                break;
            }
        }
    }
};
