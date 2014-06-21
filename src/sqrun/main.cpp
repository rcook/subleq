#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace std;

#include "common/util.hpp"
#include "libsqrun/runner.hpp"

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

static void run(const string &fileName)
{
    vector<char> instructions;
    load(fileName, instructions);
    Runner::run(instructions);
}

int main(int argc, char *argv[])
{
    try
    {
        for (auto i = 1; i < argc; ++i)
        {
            run(argv[i]);
        }
        return EXIT_SUCCESS;
    }
    catch (const exception &ex)
    {
        cout << "Message: " << ex.what() << endl;
        return EXIT_FAILURE;
    }
}

