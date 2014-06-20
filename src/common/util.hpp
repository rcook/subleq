#pragma once

#include "common/formatter.hpp"

template<typename T>
bool tryLexicalCast(const string &str, T &value)
{
    istringstream stream;
    stream.str(str);
    stream >> value;
    return stream.rdbuf()->in_avail() == 0;
}

template<typename T>
T lexicalCast(const string &str)
{
    T value;
    if (!tryLexicalCast(str, value))
    {
        throw runtime_error(Formatter() << "Could not convert " << str);
    }
    return value;
}

template<typename Target, typename Source = int>
static Target castInt(Source value)
{
    if (value < numeric_limits<Target>::min() ||
        value > numeric_limits<Target>::max())
    {
        throw out_of_range(Formatter() << "Value " << value << " out of range");
    }
    return static_cast<Target>(value);
}

