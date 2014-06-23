#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace std;

#include "common/util.hpp"
#include "libsqrun/runner.hpp"

static void load(const string &fileName, vector<char> &ops)
{
    cout << "Parsing object file " << fileName << endl;
    ifstream file(fileName);
    string token;
    while (file >> token)
    {
        ops.push_back(castInt<char>(lexicalCast<int>(token)));
    }
}

static void run(const string &fileName)
{
    vector<char> ops;
    load(fileName, ops);
    Runner::run(false, ops, map<string, int>());
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

