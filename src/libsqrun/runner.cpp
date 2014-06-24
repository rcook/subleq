#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
using namespace std;

#include "runner.hpp"
#include "common/util.hpp"

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

void Runner::run(
    const bool debug,
    const vector<char> &ops,
    const map<string, int> &symbolTable)
{
    vector<char> memory(ops);

    bool stepping = true;
    for (size_t pc = 0; pc >= 0 && pc < memory.size();)
    {
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

                string command(trim(temp, isspace));
                if (command.compare("c") == 0)
                {
                    stepping = false;
                    break;
                }
                else if (command.compare("m") == 0)
                {
                    dumpMemory(memory);
                }
                else if (command.compare("p") == 0)
                {
                    printSymbols(symbolTable, memory);
                }
                else if (command.compare("q") == 0)
                {
                    return;
                }
                else if (command.compare("r") == 0)
                {
                    cout << ": pc = " << pc << endl;
                }
                else if (command.compare("s") == 0)
                {
                    break;
                }
                else
                {
                    if (!command.empty())
                    {
                        cout << "Unrecognized command " << command << endl;
                    }
                    cout << "Available commands:" << endl
                        << "c (continue)" << endl
                        << "m (memory)" << endl
                        << "p (print)" << endl
                        << "q (quit)" << endl
                        << "r (registers)" << endl
                        << "s (step)" << endl;
                    continue;
                }
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

