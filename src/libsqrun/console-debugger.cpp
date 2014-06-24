#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

#include "console-debugger.hpp"
#include "common/util.hpp"

ConsoleDebugger::ConsoleDebugger(
    bool enabled,
    const std::map<std::string, int> &symbolTable,
    vector<char> &memory)
    :
    m_enabled(enabled),
    m_symbolTable(symbolTable),
    m_memory(memory),
    m_memorySize(memory.size()),
    m_stepping(true)
{
}

ConsoleDebugger::~ConsoleDebugger()
{
}

bool ConsoleDebugger::interrupt(size_t pc, char a, char b, char c)
{
    if (!m_enabled)
    {
        return true;
    }

    if (!m_stepping && m_breakpoints.find(pc) != m_breakpoints.end())
    {
        m_stepping = true;
    }

    if (!m_stepping)
    {
        return true;
    }

    cout << "# subleq " <<
        " A = " << prettyOperand(a) <<
        " B = " << prettyOperand(b) <<
        " C = " << static_cast<int>(c) << endl;

    for (;;)
    {
        cout << "pc = " << pc << " > ";
        string input;
        getline(cin, input);

        vector<string> tokens;
        tokenizeInput(input, tokens);

        int tokensSize = tokens.size();
        if (tokensSize == 1)
        {
            string &command(tokens[0]);

            if (command.compare("bc") == 0)
            {
                m_breakpoints.clear();
                cout << "! breakpoints cleared" << endl;
                continue;
            }
            else if (command.compare("bl") == 0)
            {
                printBreakpoints();
                continue;
            }
            else if (command.compare("c") == 0)
            {
                m_stepping = false;
                return true;
            }
            else if (command.compare("m") == 0)
            {
                printMemory();
                continue;
            }
            else if (command.compare("p") == 0)
            {
                printSymbols();
                continue;
            }
            else if (command.compare("q") == 0)
            {
                return false;
            }
            else if (command.compare("r") == 0)
            {
                cout << ": pc = " << pc << endl;
                continue;
            }
            else if (command.compare("s") == 0)
            {
                return true;
            }
        }
        else if (tokensSize == 2)
        {
            string &command(tokens[0]);
            string &op(tokens[1]);

            if (command.compare("bp") == 0)
            {
                size_t address;
                if (tryLexicalCast(op, address))
                {
                    if (address < 0 || address >= m_memorySize)
                    {
                        cout << "! address out of range" << endl;
                    }
                    else
                    {
                        auto result(m_breakpoints.insert(address));
                        size_t index(distance(m_breakpoints.begin(), result.first));
                        if (result.second)
                        {
                            cout << "! created BP" << index << " at address " << address << endl;
                        }
                        else
                        {
                            cout << "! existing BP" << index << " at address " << address << endl;
                        }
                    }
                    continue;
                }
            }
            else if (command.compare("bd") == 0)
            {
                size_t index;
                if (tryLexicalCast(op, index))
                {
                    if (index < 0 || index >= m_breakpoints.size())
                    {
                        cout << "! index out of range" << endl;
                    }
                    else
                    {
                        auto iter(m_breakpoints.begin());
                        advance(iter, index);
                        m_breakpoints.erase(iter);
                    }
                    continue;
                }
            }
        }

        if (tokensSize > 0)
        {
            cout << "Invalid command " << tokens[0] << endl;
        }
        cout << "Available commands:" << endl
            << "bc (clear breakpoints)" << endl
            << "bd <index> (delete breakpoint)" << endl
            << "bl (list breakpoints)" << endl
            << "bp <address> (set breakpoint)" << endl
            << "c (continue)" << endl
            << "m (memory)" << endl
            << "p (print symbols)" << endl
            << "q (quit)" << endl
            << "r (registers)" << endl
            << "s (step)" << endl;
    }
}

string ConsoleDebugger::prettyOperand(char op) const
{
    Formatter builder;
    builder << static_cast<int>(op);
    if (static_cast<size_t>(op) >= m_memorySize)
    {
        builder << " (?)";
    }
    else
    {
        builder << " (" << static_cast<int>(m_memory[op]) << ")";
    }
    return builder;
}

void ConsoleDebugger::tokenizeInput(const string &input, vector<string> &tokens) const
{
    istringstream stream(input);
    string token;
    while (stream >> token)
    {
        tokens.push_back(token);
    }
}

void ConsoleDebugger::printBreakpoints() const
{
    if (m_breakpoints.empty())
    {
        cout << ": no breakpoints" << endl;
    }
    else
    {
        size_t index = 0;
        for (size_t breakpoint : m_breakpoints)
        {
            cout << left << ": BP" << index << " = " << breakpoint << endl;
            ++index;
        }
    }
}

void ConsoleDebugger::printSymbols() const
{
    for (auto &pair : m_symbolTable)
    {
        int address = pair.second;
        int value = m_memory[address];
        cout << left << ": " << setw(5) << pair.first <<
            " (" << setw(5) << address <<
            ") = " << right << setw(5) << value << endl;
    }
}

void ConsoleDebugger::printMemory(const size_t columnCount) const
{
    for (size_t i = 0; i < m_memory.size(); i += columnCount)
    {
        cout << right << ": " << setw(4) << i << ":";
        auto upper = min(i + columnCount, m_memorySize);
        for (auto j = i; j < upper; ++j)
        {
            cout << " " << setw(4) << static_cast<int>(m_memory[j]);
        }
        cout << endl;
    }
}
