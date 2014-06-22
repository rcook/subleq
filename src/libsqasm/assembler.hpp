#pragma once

#include <memory>
#include <string>

#include "compilation-data.hpp"

namespace Assembler
{
    std::unique_ptr<CompilationData> assemble(const std::string &fileName);
}
