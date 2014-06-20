#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

#include "common/util.hpp"

static void load(const string &fileName, vector<char> &instructions)
{
    cout << "Parsing object file " << fileName << endl;
    ifstream file(fileName);
    string token;
    while (file >> token)
    {
        instructions.push_back(castInt<char>(lexicalCast<int>(token)));
    }
}

static void dump(const string &message, char a, char b, char c)
{
    cout << message << ":" <<
        " A = " << static_cast<int>(a) <<
        " B = " << static_cast<int>(b) <<
        " C = " << static_cast<int>(c) <<endl;
}

static void dump(const vector<char> &instructions, const size_t columnCount = 3)
{
    for (auto i = 0; i < instructions.size(); i += columnCount)
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

static void run(vector<char> &instructions, const bool trace = false)
{
    for (auto pc = 0; pc >= 0 && pc < instructions.size(); )
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
            pc = -1;
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

static void execute(const string &fileName)
{
    vector<char> instructions;
    load(fileName, instructions);
    run(instructions, false);
}

int main(int argc, char *argv[])
{
    try
    {
        for (auto i = 1; i < argc; ++i)
        {
            execute(argv[i]);
        }
        return EXIT_SUCCESS;
    }
    catch (const exception &ex)
    {
        cout << "Message: " << ex.what() << endl;
        return EXIT_FAILURE;
    }
}

