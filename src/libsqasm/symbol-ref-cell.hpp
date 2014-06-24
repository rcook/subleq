#pragma once

#include "cell.hpp"

class SymbolRefCell : public Cell
{
    SymbolRefCell() = delete;
    SymbolRefCell(const SymbolRefCell &) = delete;
    SymbolRefCell &operator=(const SymbolRefCell &) = delete;
    SymbolRefCell(SymbolRefCell &&) = delete;
    SymbolRefCell &operator=(SymbolRefCell &&) = delete;

public:
    SymbolRefCell(int address, const std::string &symbol);
    ~SymbolRefCell();
    std::string toString() const;
    int emit(const std::map<std::string, int> &symbolTable) const;

private:
    const std::string m_symbol;
};
