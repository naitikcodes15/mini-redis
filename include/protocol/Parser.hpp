#pragma once 
#include "protocol/Command.hpp"

class Parser{
    public:
    static Command parse(const string& raw_input);
};
