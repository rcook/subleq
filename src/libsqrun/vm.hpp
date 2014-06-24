#pragma once

#include <map>
#include <vector>

namespace VM
{
    void run(
        const bool debug,
        const std::vector<char> &ops,
        const std::map<std::string, int> &symbolTable);
}

