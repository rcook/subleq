// Copyright (C) 2014, Richard Cook.
// See licence at following location for details:
// https://github.com/rcook/subleq/blob/master/LICENSE

#pragma once

#include <algorithm>
#include <limits>

#include "common/formatter.hpp"

template<typename T>
bool tryLexicalCast(const std::string &str, T &value)
{
    std::istringstream stream;
    stream.str(str);
    stream >> value;
    return stream.rdbuf()->in_avail() == 0;
}

template<typename T>
T lexicalCast(const std::string &str)
{
    T value;
    if (!tryLexicalCast(str, value))
    {
        throw std::runtime_error(Formatter() << "Could not convert " << str);
    }
    return value;
}

template<typename Target, typename Source = int>
static Target castInt(Source value)
{
    if (value < std::numeric_limits<Target>::min() ||
        value > std::numeric_limits<Target>::max())
    {
        throw std::out_of_range(Formatter() << "Value " << value << " out of range");
    }
    return static_cast<Target>(value);
}

