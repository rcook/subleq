// Copyright (C) 2014, Richard Cook.
// See licence at following location for details:
// https://github.com/rcook/subleq/blob/master/LICENSE

#include <string>
using namespace std;

#include "common/formatter.hpp"
#include "value-cell.hpp"

ValueCell::ValueCell(int address, int value)
    : Cell(address),
    m_value(value)
{
}

ValueCell::~ValueCell()
{
}

string ValueCell::toString() const
{
    return Formatter() << "[value " << m_value << "]";
}

int ValueCell::emit(const map<string, int> & /*symbolTable*/) const
{
    return m_value;
}
