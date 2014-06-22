#pragma once

#include <memory>
#include <vector>

#include "cell.hpp"

class CompilationData
{
    CompilationData() = delete;
    CompilationData(const CompilationData &) = delete;
    CompilationData &operator=(const CompilationData &) = delete;
    CompilationData(CompilationData &&) = delete;
    CompilationData &operator=(CompilationData &&) = delete;

public:
    CompilationData(
        std::unique_ptr<std::map<std::string, int>> symbolTable,
        std::unique_ptr<std::vector<std::unique_ptr<Cell>>> cells,
        std::unique_ptr<std::vector<char>> ops);
    ~CompilationData();
    const std::map<std::string, int> &symbolTable() const;
    const std::vector<std::unique_ptr<Cell>> &cells() const;
    const std::vector<char> &ops() const;

private:
    std::unique_ptr<std::map<std::string, int>> m_symbolTable;
    std::unique_ptr<std::vector<std::unique_ptr<Cell>>> m_cells;
    std::unique_ptr<std::vector<char>> m_ops;
};
