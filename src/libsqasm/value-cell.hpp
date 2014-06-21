#pragma once

#include "cell.hpp"

class ValueCell : public Cell
{
    ValueCell();
    ValueCell(const ValueCell &) = delete;
    ValueCell &operator=(const ValueCell &) = delete;
    ValueCell(ValueCell &&) = delete;
    ValueCell &operator=(ValueCell &&) = delete;

public:
    ValueCell(int address, int value);
    ~ValueCell();
    std::string toString() const;
    int emit(const std::map<std::string, int> &symbolTable) const;

private:
    const int m_value;
};
