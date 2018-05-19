#pragma once

template<typename TType>
class breakpoint
{
public:
    TType type;

    breakpoint() = default;
    explicit breakpoint(const TType& type) : type(type)
    {
    }
};

enum class breakpoint_mem_type
{
    none = 0,
    walk = 1,
    increment = 2,
    decrement = 4,
    set = 8,
    change = increment | decrement | set
};

enum class breakpoint_instr_type
{
    none = 0,
    execute = 1
};

using breakpoint_mem = breakpoint<breakpoint_mem_type>;
using breakpoint_instr = breakpoint<breakpoint_instr_type>;

using breakpoint_mem_map = std::unordered_map<size_t, breakpoint_mem>;
using breakpoint_instr_map = std::unordered_map<size_t, breakpoint_instr>;