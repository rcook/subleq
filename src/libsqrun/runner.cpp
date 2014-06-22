#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

#include "runner.hpp"
#include "common/util.hpp"

static void dump(const string &message, char a, char b, char c)
{
    cout << message << ":" <<
        " A = " << static_cast<int>(a) <<
        " B = " << static_cast<int>(b) <<
        " C = " << static_cast<int>(c) <<endl;
}

static void dump(const vector<char> &instructions, const size_t columnCount = 3)
{
    for (size_t i = 0; i < instructions.size(); i += columnCount)
    {
        cout << setw(4) << i << ":";
        auto upper = min(i + columnCount, instructions.size());
        for (auto j = i; j < upper; ++j)
        {
            cout << " " << setw(4) << static_cast<int>(instructions[j]);
        }
        cout << endl;
    }
}

void Runner::run(vector<char> &instructions, const bool trace)
{
    for (size_t pc = 0; pc >= 0 && pc < instructions.size(); )
    {
        if (trace)
        {
            cout << "pc = " << pc << endl;
            dump(instructions);
        }

        auto a = instructions[pc];
        auto b = instructions[pc + 1];
        auto c = instructions[pc + 2];

        if (a < 0 || b < -1)
        {
            break;
        }
        else if (b == -1)
        {
            cout << instructions[a];
            pc = c;
        }
        else
        {
            if (trace)
            {
                dump("subleq", instructions[a], instructions[b], c);
            }
            instructions[b] -= instructions[a];
            if (instructions[b] > 0)
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

