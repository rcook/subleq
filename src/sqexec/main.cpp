#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include "libsqasm/assembler.hpp"
#include "libsqrun/runner.hpp"

static void assemble(const string &fileName, const bool debug)
{
    auto compilationData(Assembler::assemble(fileName));
    Runner::run(
        debug,
        compilationData->ops(),
        compilationData->symbolTable());
}

int main(int argc, char *argv[])
{
    try
    {
        bool debug = false;

        for (auto i = 1; i < argc; ++i)
        {
            string arg(argv[i]);

            if (i == 1 && arg.compare("--debug") == 0)
            {
                debug = true;
                continue;
            }

            assemble(arg, debug);
        }
        return EXIT_SUCCESS;
    }
    catch (const exception &ex)
    {
        cout << "Message: " << ex.what() << endl;
        return EXIT_FAILURE;
    }
}

