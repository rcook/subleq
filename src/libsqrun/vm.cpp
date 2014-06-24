#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <set>
#include <stdexcept>
#include <string>
using namespace std;

#include "vm.hpp"
#include "common/util.hpp"

static void listBreakpoints(const set<size_t> &breakpoints)
{
    if (breakpoints.empty())
    {
        cout << ": no breakpoints" << endl;
    }
    else
    {
        size_t index = 0;
        for (size_t breakpoint : breakpoints)
        {
            cout << left << ": BP" << index << " = " << breakpoint << endl;
            ++index;
        }
    }
}

static void printSymbols(const map<string, int> &symbolTable, const vector<char> &memory)
{
    for (auto &pair : symbolTable)
    {
        int address = pair.second;
        int value = memory[address];
        cout << left << ": " << setw(5) << pair.first <<
            " (" << setw(5) << address <<
            ") = " << right << setw(5) << value << endl;
    }
}

static void dumpMemory(const vector<char> &memory, const size_t columnCount = 3)
{
    for (size_t i = 0; i < memory.size(); i += columnCount)
    {
        cout << right << ": " << setw(4) << i << ":";
        auto upper = min(i + columnCount, memory.size());
        for (auto j = i; j < upper; ++j)
        {
            cout << " " << setw(4) << static_cast<int>(memory[j]);
        }
        cout << endl;
    }
}

static void tokenizeDebugInput(const string &command, vector<string> &tokens)
{
    istringstream stream(command);
    string temp;
    while (stream >> temp)
    {
        tokens.push_back(temp);
    }
}

void VM::run(
    const bool debug,
    const vector<char> &ops,
    const map<string, int> &symbolTable)
{
    vector<char> memory(ops);
    size_t memorySize(memory.size());

    set<size_t> breakpoints;

    bool stepping = true;
    for (size_t pc = 0; pc >= 0 && pc < memorySize;)
    {
        if (debug && breakpoints.find(pc) != breakpoints.end())
        {
            stepping = true;
        }

        auto a = memory[pc];
        auto b = memory[pc + 1];
        auto c = memory[pc + 2];

        if (debug && stepping)
        {
            cout << "# subleq " <<
                " A = " << static_cast<int>(a) <<
                " B = " << static_cast<int>(b) <<
                " C = " << static_cast<int>(c) << endl;

            for (;;)
            {
                cout << "pc = " << pc << " > ";
                string temp;
                getline(cin, temp);

                vector<string> tokens;
                tokenizeDebugInput(temp, tokens);

                int tokensSize = tokens.size();
                if (tokensSize == 1)
                {
                    string &command(tokens[0]);

                    if (command.compare("bc") == 0)
                    {
                        breakpoints.clear();
                        cout << "! breakpoints cleared" << endl;
                        continue;
                    }
                    else if (command.compare("bl") == 0)
                    {
                        listBreakpoints(breakpoints);
                        continue;
                    }
                    else if (command.compare("c") == 0)
                    {
                        stepping = false;
                        break;
                    }
                    else if (command.compare("m") == 0)
                    {
                        dumpMemory(memory);
                        continue;
                    }
                    else if (command.compare("p") == 0)
                    {
                        printSymbols(symbolTable, memory);
                        continue;
                    }
                    else if (command.compare("q") == 0)
                    {
                        return;
                    }
                    else if (command.compare("r") == 0)
                    {
                        cout << ": pc = " << pc << endl;
                        continue;
                    }
                    else if (command.compare("s") == 0)
                    {
                        break;
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
                            if (address < 0 || address >= memorySize)
                            {
                                cout << "! address out of range" << endl;
                            }
                            else
                            {
                                auto result(breakpoints.insert(address));
                                size_t index(distance(breakpoints.begin(), result.first));
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
                            if (index < 0 || index >= breakpoints.size())
                            {
                                cout << "! index out of range" << endl;
                            }
                            else
                            {
                                auto iter(breakpoints.begin());
                                advance(iter, index);
                                breakpoints.erase(iter);
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
                    << "p (print)" << endl
                    << "q (quit)" << endl
                    << "r (registers)" << endl
                    << "s (step)" << endl;
                continue;
            }
        }

        if (a < 0 || b < -1)
        {
            break;
        }
        else if (b == -1)
        {
            cout << memory[a];
            pc = c;
        }
        else
        {
            memory[b] -= memory[a];
            if (memory[b] > 0)
            {
                pc += 3;
            }
            else
            {
                pc = c;
            }
        }
    }
}

