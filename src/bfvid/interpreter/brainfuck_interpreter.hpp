#pragma once

#include <istream>
#include <ostream>
#include <stack>
#include <array>
#include <unordered_map> 
#include <functional>

//template<typename TMemoryContainer = std::array<int8_t, 256>>
template<typename TData = int8_t, size_t VMemSize = 256>
class brainfuck_interpreter
{
public:
    constexpr static size_t mem_size = VMemSize;
    using cell_t = TData;
    using memory_t = std::array<cell_t, mem_size>;
    //using memory_t = TMemoryContainer;

private:
    std::unordered_map<size_t, size_t> cycle_ends;
public:

    const std::string program;
    std::ostream* const out;
    std::istream* const in;

    memory_t memory;
    size_t mem_ptr;
    size_t instr_ptr;

    std::stack<size_t> loop_stack;

    std::function<void(brainfuck_interpreter<TData, VMemSize>&, char, char, char)> step_callback;

public:
    void scan_cycle_ends()
    {
        while(instr_ptr < program.length())
        {
            const auto instruction = program[instr_ptr++];
            switch (instruction)
            {
            case '[':
                loop_stack.push(instr_ptr);
                break;
            case ']':
                auto start = loop_stack.top();
                loop_stack.pop();
                cycle_ends[start] = instr_ptr;
                break;
            }
        }
        
        instr_ptr = 0;
    }

    explicit brainfuck_interpreter(const std::string& program_str, std::ostream* const out = nullptr, std::istream* const in = nullptr) :
        program(program_str),
        out(out),
        in(in), 
        memory{}, 
        mem_ptr(0),
        instr_ptr(0),
        step_callback(nullptr)
    {
        scan_cycle_ends();
    }

    bool step()
    {
        if (instr_ptr >= program.length())
        {
            return false;
        }
        const auto instruction = program[instr_ptr++];
        auto executed = true;
        char in_ch = '\0';
        char out_ch = '\0';
        switch (instruction)
        {
        case '>':
            ++mem_ptr;
            break;
        case '<':
            --mem_ptr;
            break;
        case '+':
            ++memory[mem_ptr];
            break;
        case '-':
            --memory[mem_ptr];
            break;
        case '.':
            if(out)
            {
                out_ch = memory[mem_ptr];
                *out << static_cast<char>(out_ch);
            }
            break;
        case ',':
            if (in == nullptr)
            {
                throw std::runtime_error("No input stream provided, unable to read data.");
            }
            *in >> in_ch;
            memory[mem_ptr] = in_ch;
            break;
        case '[':
            if(memory[mem_ptr])
            {
                loop_stack.push(instr_ptr);
            }
            else
            {
                instr_ptr = cycle_ends.find(instr_ptr)->second;
            }
            break;
        case ']':
            if (memory[mem_ptr])
            {
                const auto jmp_back_pos = loop_stack.top();
                instr_ptr = jmp_back_pos;
            }
            break;
        default:
            executed = false;
            break;
        }

        if(executed && step_callback)
        {
            step_callback(*this, instruction, in_ch, out_ch);
        }

        return true;
    }

    void run()
    {
        while(step())
        {
        }
    }
};
