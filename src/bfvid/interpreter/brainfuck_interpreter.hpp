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

    void toggle_instr_break(const size_t ptr)
    {
        if(is_instr_break_set(ptr))
        {
            remove_instr_break(ptr);
        }
        else
        {
            set_instr_break(ptr);
        }
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

    bool is_instr_break_set(const size_t ptr)
    {
        return breakpoints_instr.find(ptr) != breakpoints_instr.end();
    }

    void remove_instr_break(const size_t ptr)
    {
        breakpoints_instr.erase(ptr);
    }

    bool step_over()
    {
        if(this->is_end())
        {
            return false;
        }
        auto instr = this->get_next_instruction();
        if(instr != '[')
        {
            return this->step();
        }
        breakpoint_loop_set = true;
        breakpoint_loop = this->get_loop_end_for(this->instr_ptr + 1);
        run();
        return !this->is_end();
    }

    bool step_out()
    {
        if (this->is_end())
        {
            return false;
        }
        if(!this->loop_stack.empty())
        {
            auto loop_start_idx = this->loop_stack.top();
            breakpoint_loop_set = true;
            breakpoint_loop = this->get_loop_end_for(loop_start_idx);
        }
        return run();
    }

    bool run()
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
        breakpoint_loop_set = false;
        return !this->is_end();
    }

    bool run_to_end()
    {
        while (this->step())
        {
            
        }
        breakpoint_loop_set = false;
        return false;
    }
};
