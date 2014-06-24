#pragma once

#include <map>
#include <set>

class ConsoleDebugger
{
    ConsoleDebugger() = delete;
    ~ConsoleDebugger() = delete;
    ConsoleDebugger(const ConsoleDebugger &) = delete;
    ConsoleDebugger &operator=(const ConsoleDebugger &) = delete;
    ConsoleDebugger(ConsoleDebugger &&) = delete;
    ConsoleDebugger &operator=(ConsoleDebugger &&) = delete;

public:
    ConsoleDebugger(bool enabled, const std::map<std::string, int> &symbolTable, vector<char> &memory);

public:
    bool interrupt(size_t pc, char a, char b, char c);

private:
    void tokenizeInput(const string &input, vector<string> &tokens) const;
    void printBreakpoints() const;
    void printSymbols() const;
    void printMemory(const size_t columnCount = 3) const;

private:
    bool m_enabled;
    const std::map<std::string, int> &m_symbolTable;
    const vector<char> &m_memory;
    const size_t m_memorySize;
    bool m_stepping;
    std::set<size_t> m_breakpoints;
};
