#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include "common/formatter.hpp"
#include "libsqasm/assembler.hpp"

static void emit(const string &fileName, const vector<char> &instructions)
{
    cout << "Emitting object file " << fileName << endl;

    ofstream file(fileName);
    for (auto instruction : instructions)
    {
        file << setw(5) << static_cast<int>(instruction) << endl;
    }
}

static void assemble(const string &fileName)
{
    vector<char> instructions;
    Assembler::assemble(fileName, instructions);

    string outputFileName = Formatter() << fileName << "obj";
    emit(outputFileName, instructions);
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

