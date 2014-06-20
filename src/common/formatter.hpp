#pragma once

#include <sstream>

// http://stackoverflow.com/questions/12261915/howto-throw-stdexceptions-with-variable-messages
class Formatter
{
    Formatter(const Formatter &) = delete;
    Formatter &operator=(const Formatter &) = delete;
    Formatter(Formatter &&) = delete;
    Formatter &operator=(Formatter &&) = delete;

public:
    enum ConvertToString { to_str };

public:
    Formatter() { }

    ~Formatter() { }

    operator std::string() const { return m_stream.str(); }

    template <typename T>
    Formatter &operator <<(const T &item)
    {
        m_stream << item;
        return *this;
    }

    std::string operator >> (ConvertToString) { return m_stream.str(); }

    std::string str() const { return m_stream.str(); }

private:
    std::stringstream m_stream;
};

