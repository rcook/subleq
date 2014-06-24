// Copyright (C) 2014, Richard Cook.
// See licence at following location for details:
// https://github.com/rcook/subleq/blob/master/LICENSE

#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace std;

#include "common/util.hpp"
#include "libsqrun/vm.hpp"

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
    VM::run(false, ops, map<string, int>());
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

