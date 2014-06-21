#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <stdexcept>
#include <vector>
using namespace std;

#include "common/formatter.hpp"
#include "common/util.hpp"

static void tokenize(const string &fileName, vector<string> &tokens)
{
    cout << "Tokenizing " << fileName << endl;
    ifstream file(fileName);
    string token;
    while (file >> token)
    {
        tokens.push_back(token);
    }
}

static void getAddresses(const vector<string> &tokens, map<string, char> &addresses)
{
    cout << "Getting addresses" << endl;
    for (size_t address = 0; address < tokens.size(); ++address)
    {
        auto &token = tokens[address];
        auto colonIndex = token.find(':');
        if (colonIndex != string::npos)
        {
            auto symbol(token.substr(0, colonIndex));
            addresses.insert(make_pair(symbol, address));
        }
    }
}

static void generateCode(const vector<string> &tokens, const map<string, char> &addresses, vector<char> &instructions)
{
    cout << "Generating code" << endl;
    for (size_t address = 0; address < tokens.size(); ++address)
    {
        auto &token = tokens[address];
        auto colonIndex = token.find(':');
        if (colonIndex == string::npos)
        {
            int value;
            if (tryLexicalCast(token, value))
            {
                instructions.push_back(castInt<char>(value));
            }
            else
            {
                auto iter(addresses.find(token));
                if (iter == addresses.end())
                {
                    throw runtime_error(Formatter() <<
                        "No such symbol \"" << token << "\" at address " << address);
                }
                else
                {
                    instructions.push_back(iter->second);
                }
            }
        }
        else
        {
            auto valueStr(token.substr(colonIndex + 1));
            auto value = lexicalCast<int>(valueStr);
            instructions.push_back(castInt<char>(value));
        }
    }
}

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
    vector<string> tokens;
    tokenize(fileName, tokens);

    map<string, char> addresses;
    getAddresses(tokens, addresses);

    vector<char> instructions;
    generateCode(tokens, addresses, instructions);

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

