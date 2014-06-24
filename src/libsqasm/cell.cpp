// Copyright (C) 2014, Richard Cook.
// See licence at following location for details:
// https://github.com/rcook/subleq/blob/master/LICENSE

#include "cell.hpp"

Cell::Cell(int address)
    : m_address(address)
{
}

Cell::~Cell()
{
}

int Cell::address() const
{
    return m_address;
}
