// Copyright (C) 2014, Richard Cook.
// See licence at following location for details:
// https://github.com/rcook/subleq/blob/master/LICENSE

#pragma once

#include <map>
#include <string>

class Cell
{
    Cell() = delete;
    Cell(const Cell &) = delete;
    Cell &operator=(const Cell &) = delete;
    Cell(Cell &&) = delete;
    Cell &operator=(Cell &&) = delete;

public:
    virtual ~Cell();
    virtual std::string toString() const = 0;
    virtual int emit(const std::map<std::string, int> &symbolTable) const = 0;
    int address() const;

protected:
    Cell(int address);

private:
    const int m_address;
};
