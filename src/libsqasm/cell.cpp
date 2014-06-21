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
