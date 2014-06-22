#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include "common/formatter.hpp"
#include "libsqasm/assembler.hpp"

static void emit(const string &fileName, const vector<char> &ops)
{
    cout << "Emitting object file " << fileName << endl;

    ofstream file(fileName);
    for (auto op : ops)
    {
        file << setw(5) << static_cast<int>(op) << endl;
    }
}

static void assemble(const string &fileName)
{
    vector<char> ops;
    Assembler::assemble(fileName, ops);

    string outputFileName = Formatter() << fileName << "obj";
    emit(outputFileName, ops);
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

