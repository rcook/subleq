#pragma once

#include <limits>

#include "common/formatter.hpp"

template<typename Predicate>
string trim(const string &str, Predicate isspacePredicate)
{
    auto beginIter(find_if_not(str.begin(), str.end(), isspacePredicate));
    auto endIter(find_if(beginIter, str.end(), isspacePredicate));
    return string(beginIter, endIter);
}

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

