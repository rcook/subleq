// Copyright (C) 2014, Richard Cook.
// See licence at following location for details:
// https://github.com/rcook/subleq/blob/master/LICENSE

#pragma once

#include "cell.hpp"

class ValueCell : public Cell
{
    ValueCell() = delete;
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
