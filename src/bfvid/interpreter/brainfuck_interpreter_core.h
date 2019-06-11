#pragma once

//template<typename TMemoryContainer = std::array<int8_t, 256>>
template<typename TData = int8_t, size_t VMemSize = 256>
class brainfuck_interpreter_core
{
public:
    constexpr static size_t mem_size = VMemSize;
    using cell_t = TData;
    using memory_t = std::array<cell_t, mem_size>;
    //using memory_t = TMemoryContainer;

protected:
    std::unordered_map<size_t, size_t> cycle_ends;

    size_t get_loop_end_for(const size_t ptr)
    {
        return cycle_ends.find(ptr)->second;
    }
public:

    const std::string program;
    std::ostream* const out;
    std::istream* const in;

    int all_steps;
    int instr_steps;

    memory_t memory;
    int32_t mem_ptr;
	size_t fixed_mem_ptr;
    size_t instr_ptr;

    std::stack<size_t> loop_stack;

    std::function<void(brainfuck_interpreter_core<TData, VMemSize>&, char, char, char)> step_callback;

public:
    void scan_cycle_ends()
    {
        while (instr_ptr < program.length())
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

    explicit brainfuck_interpreter_core(const std::string& program_str, std::ostream* const out = nullptr, std::istream* const in = nullptr) :
        program(program_str),
        out(out),
        in(in),
        all_steps(0),
        instr_steps(0),
        memory{},
		mem_ptr(0),
		fixed_mem_ptr(0),
        instr_ptr(0),
        step_callback(nullptr)
    {
        scan_cycle_ends();
    }

    bool is_end() const
    {
        return instr_ptr >= program.length();
    }

    char get_next_instruction() const
    {
        return program[instr_ptr];
    }

	size_t fix_mem_ptr(int32_t mem_ptr)
    {
	    if(mem_ptr >= 0)
	    {
			return mem_ptr;
	    }

		return mem_size + mem_ptr;
    }

    bool step()
    {
        if (is_end())
        {
            return false;
        }
        const auto instruction = get_next_instruction();
        ++instr_ptr;
        auto executed = true;
        char in_ch = '\0';
        char out_ch = '\0';
        switch (instruction)
        {
        case '>':
            ++mem_ptr;
			fixed_mem_ptr = fix_mem_ptr(mem_ptr);
            break;
        case '<':
            --mem_ptr;
			fixed_mem_ptr = fix_mem_ptr(mem_ptr);
            break;
        case '+':
            ++memory[fixed_mem_ptr];
            break;
        case '-':
            --memory[fixed_mem_ptr];
            break;
        case '.':
            if (out)
            {
                out_ch = memory[fixed_mem_ptr];
                *out << static_cast<char>(out_ch);
            }
            break;
        case ',':
            if (in == nullptr)
            {
                throw std::runtime_error("No input stream provided, unable to read data.");
            }
            *in >> in_ch;
            memory[fixed_mem_ptr] = in_ch;
            break;
        case '[':
            if (memory[fixed_mem_ptr])
            {
                loop_stack.push(instr_ptr);
            }
            else
            {
                instr_ptr = get_loop_end_for(instr_ptr);
            }
            break;
        case ']':
        	if (memory[fixed_mem_ptr])
            {
                const auto jmp_back_pos = loop_stack.top();
                instr_ptr = jmp_back_pos;
            }
			else
			{
				loop_stack.pop();
			}
            break;
        default:
            executed = false;
            break;
        }

        all_steps++;

        if (executed)
        {
            instr_steps++;
            if (step_callback)
            {
                step_callback(*this, instruction, in_ch, out_ch);
            }
        }

        return true;
    }

    void reset()
    {
        all_steps = 0;
        instr_steps = 0;
        memory = {};
		mem_ptr = 0;
		fixed_mem_ptr = 0;
        instr_ptr = 0;
        loop_stack = {};
    }
};