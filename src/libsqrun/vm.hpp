// Copyright (C) 2014, Richard Cook.
// See licence at following location for details:
// https://github.com/rcook/subleq/blob/master/LICENSE

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

