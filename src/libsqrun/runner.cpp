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

static void dump(const vector<char> &ops, const size_t columnCount = 3)
{
    for (size_t i = 0; i < ops.size(); i += columnCount)
    {
        cout << setw(4) << i << ":";
        auto upper = min(i + columnCount, ops.size());
        for (auto j = i; j < upper; ++j)
        {
            cout << " " << setw(4) << static_cast<int>(ops[j]);
        }
        cout << endl;
    }
}

void Runner::run(const vector<char> &ops, const bool debug)
{
    vector<char> mutableOps(ops);

    for (size_t pc = 0; pc >= 0 && pc < mutableOps.size();)
    {
        if (debug)
        {
            cout << "pc = " << pc << endl;
            dump(mutableOps);
        }

        auto a = mutableOps[pc];
        auto b = mutableOps[pc + 1];
        auto c = mutableOps[pc + 2];

        if (a < 0 || b < -1)
        {
            break;
        }
        else if (b == -1)
        {
            cout << mutableOps[a];
            pc = c;
        }
        else
        {
            if (debug)
            {
                dump("subleq", mutableOps[a], mutableOps[b], c);
            }
            mutableOps[b] -= mutableOps[a];
            if (mutableOps[b] > 0)
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

