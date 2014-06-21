#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include "libsqasm/assembler.hpp"
#include "libsqrun/runner.hpp"

static void assemble(const string &fileName)
{
    vector<char> instructions;
    Assembler::assemble(fileName, instructions);
    Runner::run(instructions);
}

int main(int argc, char *argv[])
{
    try
    {
        for (auto i = 1; i < argc; ++i)
        {
            assemble(argv[i]);
        }
        return EXIT_SUCCESS;
    }
    catch (const exception &ex)
    {
        cout << "Message: " << ex.what() << endl;
        return EXIT_FAILURE;
    }
}

