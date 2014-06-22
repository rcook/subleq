#pragma once

#include <map>
#include <memory>
#include <vector>

#include "compilation-data.hpp"

class CompilationDataBuilder
{
    CompilationDataBuilder(const CompilationDataBuilder &) = delete;
    CompilationDataBuilder &operator=(const CompilationDataBuilder &) = delete;
    CompilationDataBuilder(CompilationDataBuilder &&) = delete;
    CompilationDataBuilder &operator=(CompilationDataBuilder &&) = delete;

public:
    CompilationDataBuilder();
    ~CompilationDataBuilder();
    std::map<std::string, int> &symbolTable();
    std::vector<std::unique_ptr<Cell>> &cells();
    std::vector<char> &ops();
    unique_ptr<CompilationData> create();

private:
    unique_ptr<std::map<std::string, int>> m_symbolTable;
    unique_ptr<std::vector<std::unique_ptr<Cell>>> m_cells;
    unique_ptr<std::vector<char>> m_ops;
};
