#pragma once

#include <string>
#include <vector>

namespace Assembler
{
    void assemble(
        const std::string &fileName,
        std::vector<char> &instructions);
}

