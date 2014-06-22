#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include "libsqasm/assembler.hpp"
#include "libsqrun/runner.hpp"

static void assemble(const string &fileName, const bool debug = false)
{
    auto compilationData(Assembler::assemble(fileName));
    Runner::run(compilationData->ops());

    if (debug)
    {
        if (!compilationData->symbolTable().empty())
        {
            cout << "Symbol table" << endl;
            for (auto &pair : compilationData->symbolTable())
            {
                cout << "  symbol: " << pair.first << " = " << pair.second << endl;
            }
        }

        if (!compilationData->cells().empty())
        {
            cout << "Cells" << endl;
            for (auto &cell : compilationData->cells())
            {
                cout << "  cell: " << cell->toString() << endl;
            }
        }
    }
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

