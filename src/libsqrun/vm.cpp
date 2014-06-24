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
#include "console-debugger.hpp"
#include "common/util.hpp"

void VM::run(
    const bool debug,
    const vector<char> &ops,
    const map<string, int> &symbolTable)
{
    vector<char> memory(ops);
    size_t memorySize(memory.size());

    ConsoleDebugger *debugger = new ConsoleDebugger(debug, symbolTable, memory);

    for (size_t pc = 0; pc >= 0 && pc < memorySize;)
    {
        auto a = memory[pc];
        auto b = memory[pc + 1];
        auto c = memory[pc + 2];

        if (!debugger->interrupt(pc, a, b, c))
        {
            break;
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

